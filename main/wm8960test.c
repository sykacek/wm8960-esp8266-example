#include <stdio.h>
#include <math.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <nvs_flash.h>
#include <esp_spi_flash.h>
#include <esp_log.h>
#include <esp_timer.h>
#include <esp_err.h>
#include <esp_task_wdt.h>
#include <esp_system.h>
#include <esp8266/pin_mux_register.h>

#include <driver/i2c.h>
#include <driver/i2s.h>

#include "wm8960.h"

#define SDA_PIN             (4)
#define SCL_PIN             (5)
#define LED_PIN             (16)
#define I2C_STRETCH_TICK    (0)

#define AUDIO_FREQ		(441)
#define SAMPLE_RATE		(46875)

#define WM8960_I2C_ADDRESS	(0x1a)

/**
 * _______________________________________________________________________________________
 * | start | address + RW bit | ACK_LOW | REG_ADDRESS  + BIT 8 | ACK_LOW | DATA 7:0 | end
 * |-------|------------------|---------|----------------------|---------|----------|---- 
 * 
 */

static int16_t buffer[100 * 4];

static void fillBuffer(void){
	for(int i = 0; i < 200; ++i){
		buffer[i * 2] = INT16_MAX * sinf((float)i * AUDIO_FREQ / SAMPLE_RATE * 2 * 3.1415);
		buffer[i * 2 + 1] = buffer[i * 2];
	}
}

static WM_STATUS wm_write_reg(uint8_t addr, uint8_t reg, uint16_t data){
	esp_err_t ret = ESP_OK;
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();

	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (addr << 1), 1);
	i2c_master_write_byte(cmd, (reg << 1) | ((data >> 8) & 0x1), 1);
	i2c_master_write_byte(cmd, (data & 0xFF), 1);
	i2c_master_stop(cmd);

	ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, 10 / portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);

	if(ret){
		ESP_LOGI("i2c", "error: failed to write at register with %d\n", ret);
		return WM_ERROR_GENERIC;
	}

	return WM_OK;
};

static void freertos_delay_ms(uint32_t millis){
	millis /= portTICK_PERIOD_MS;

	vTaskDelay(millis);
}

static void i2c_init(void *params){
	i2c_config_t config = {
		.mode = I2C_MODE_MASTER,
		.sda_io_num = SDA_PIN,
		.scl_io_num = SCL_PIN,
		.sda_pullup_en = GPIO_PULLUP_ENABLE,
		.scl_pullup_en = GPIO_PULLUP_ENABLE,
		.clk_stretch_tick = I2C_STRETCH_TICK,
	};

	ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER));
	ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &config));

}

static void i2s_init(void *params){
	/* use GPIO 14 (SPI SCLK) for I2S master clock */
	//PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, FUNC_CLK_XTAL);

	/* use GPIO 12 (SPI MISO)for word select */
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_I2SO_WS);

	/*use GPIO 13 (SPI MOSI) for serial clock */
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_I2SO_BCK);

	/* use GPIO 15 (SPI CK) for serial data */
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U, FUNC_I2SO_DATA);

	i2s_config_t i2s_config = {
		.mode = I2S_MODE_MASTER | I2S_MODE_TX,
		.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
		.communication_format = I2S_COMM_FORMAT_I2S_MSB,
		.sample_rate = 48000,
		.tx_desc_auto_clear = 0,
		.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
		.dma_buf_count = 6,
		.dma_buf_len = 256,
	};
	
	const i2s_pin_config_t i2s_pin_config = {
		.bck_o_en = 1,
		.ws_o_en = 1,
		.data_out_en = 1,
		.data_in_en = 0,
	};

	ESP_ERROR_CHECK(i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL));
	ESP_ERROR_CHECK(i2s_set_pin(I2S_NUM_0, &i2s_pin_config));

	i2s_set_clk(I2S_NUM_0, SAMPLE_RATE, 16, I2S_CHANNEL_STEREO);

}

static void nvs_init(void *param){
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);
}

void empty_task(void *arg){
	while(1){
		esp_task_wdt_reset();
        	vTaskDelay(4000 / portTICK_PERIOD_MS);
	}
}

static void readTask(void *arg){
	uint16_t value = 0x01aa;
	uint8_t led = 1;
	while(1){
		gpio_set_level(LED_PIN, led);
		led = !led;

		ESP_ERROR_CHECK(i2s_write(I2S_NUM_0, (void *)buffer, 800, (size_t *)&value, 1000/portTICK_PERIOD_MS));
		ESP_LOGI("debug", "i2s write is %x\n", value);

		vTaskDelay(2000 / portTICK_PERIOD_MS);
	}
}

static wm8960_input_ctl_t input_ctl = WM8960_INPUT_CTL_DEFAULT;
static wm8960_output_ctl_t output_ctl = WM8960_OUTPUT_CTL_DEFAULT;

static wm8960_t chip = {
	.write_reg = wm_write_reg,
	.delay_ms = freertos_delay_ms,
	.ctl = {
		.addr = 0x1a,
		.word = WM_WORD_16BIT,
		.fmt = WM_FMT_I2S,
		.input = &input_ctl,
		.output = &output_ctl,
	},
	.memory = WM8960_MEMORY_DEFAULT,	
};

void app_main(void){
	nvs_init(NULL);
	fillBuffer();

	gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
	i2c_init(NULL);
	i2s_init(NULL);
	freertos_delay_ms(1000);

	WMinit(&chip);

	xTaskCreatePinnedToCore(readTask, "read task", configMINIMAL_STACK_SIZE, NULL, 3, NULL, PRO_CPU_NUM);
}
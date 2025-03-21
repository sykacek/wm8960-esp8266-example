#include "wm8960.h"

// static internal function for writing and creating a memory copy
static WM_STATUS write(wm8960_t *wm8960, uint8_t reg, uint16_t data){
	if(wm8960->memory.reg[reg] == WM8960_MEMORY_RESERVED){
		printf("error: failed to write to i2c bus\n");
		return WM_ERROR_NOT_PERMITTED;
	}

	wm8960->memory.reg[reg] = data;

	return wm8960->write_reg(wm8960->ctl.addr, reg, data);
}

WM_STATUS WMreset(wm8960_t *wm8960){
	// no need to create a memory copy
	WM_ERROR_CHECK(wm8960->write_reg(wm8960->ctl.addr, WM8960_RESET, 0x01));
	wm8960->memory = WM8960_MEMORY_DEFAULT;

	return WM_OK;
}

WM_STATUS WMinit(wm8960_t *wm8960){
	if(!wm8960)
		return WM_ERROR_NULLPTR;

	if(wm8960->delay_ms == NULL){
		fprintf(stderr, "delay_ms not defined, aborting.\n");
		return WM_ERROR_NULLPTR;
	}

	if(wm8960->write_reg == NULL){
		fprintf(stderr, "write_reg not defined, aborting.\n");
		return WM_ERROR_NULLPTR;
	}

	if(wm8960->ctl.input == NULL && wm8960->ctl.output){
		fprintf(stderr, "output and input are not defined, nothing to do, aborting\n");
		return WM_ERROR_INVALID_PARAM;
	}

	WM_ERROR_CHECK(write(wm8960->ctl.addr,  \ 
			WM8960_POWER1, WM8960_VMID_NORMAL | WM8960_VREF_EN));
		
	WM_ERROR_CHECK()

	/* init audio recording */
	if(wm8960->ctl.input){
		// start power
		uint16_t flag = WM8960_PWR_VREF_EN | WM8960_PWR_ADCL_EN | WM8960_PWR_ADCR_EN;
		WM_ERROR_CHECK(write(wm8960, WM8960_POWER1, flag));

		flag = WM8960_PWR_LMIC_EN | WM8960_PWR_RMIC_EN;
		WM_ERROR_CHECK(write(wm8960, WM8960_POWER3, flag));
	}

	/* init audio playback */
	if(wm8960->ctl.output){
		uint16_t flag = 0x78;
		WM_ERROR_CHECK(write(wm8960, WM8960_POWER2, flag));

		flag = WM8960_PWR_LOMIX_EN | WM8960_PWR_ROMIX_EN;
		WM_ERROR_CHECK(write(wm8960, WM8960_POWER3, flag));

		flag = 0xc0;
		WM_ERROR_CHECK(write(wm8960, WM8960_CLASSD1, flag));

		// set volume to -20 dB
		flag = WM_GAIN_dB(-20) | WM8960_DACVU;
		WM_ERROR_CHECK(write(wm8960, WM8960_LDAC, flag));
		WM_ERROR_CHECK(write(wm8960, WM8960_RDAC, flag));

		// set deemphasis
		WM_ERROR_CHECK(write(wm8960, WM8960_DACCTL1, wm8960->ctl.output->deemph));

		if(wm8960->ctl.output->monoMix == WM_TRUE)
			WM_ERROR_CHECK(write(wm8960, WM8960_ADDCTL1, 0x10));	

		WM_ERROR_CHECK(write(wm8960, WM8960_3D, ((wm8960->ctl.output->reverb & 0x0F) << 1) | 1));
	}
	
	return WM_OK;
}

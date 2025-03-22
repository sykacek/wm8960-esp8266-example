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

static uint16_t read(wm8960_t *wm8960, uint8_t reg){
	return wm8960->memory.reg[reg];
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

	/* power up */
	WM_ERROR_CHECK(write(wm8960, WM8960_POWER1, WM8960_POWER1_DEFAULT));

	/* start clocks with 24 MHz crystal, refer to documentation */
	WM_ERROR_CHECK(write(wm8960, WM8960_PLL1, 0x08));
	WM_ERROR_CHECK(write(wm8960, WM8960_PLL2, 0x31));
	WM_ERROR_CHECK(write(wm8960, WM8960_PLL3, 0x26));
	WM_ERROR_CHECK(write(wm8960, WM8960_PLL4, 0xe8));

	/* audio fmt */
	uint16_t flag;

	/* init audio recording */
	if(wm8960->ctl.input){
		// start power
		flag = WM8960_VREF | WM8960_PWR_ADCL | WM8960_PWR_ADCR;
		WM_ERROR_CHECK(write(wm8960, WM8960_POWER1, flag));

		flag = WM8960_PWR_LMIC | WM8960_PWR_RMIC;
		WM_ERROR_CHECK(write(wm8960, WM8960_POWER3, flag));
	}

	/* init audio playback */
	if(wm8960->ctl.output){
		flag = WM8960_POWER2_DEFAULT;
		WM_ERROR_CHECK(write(wm8960, WM8960_POWER2, flag));

		flag = WM8960_POWER3_DEFAULT_OUT;
		WM_ERROR_CHECK(write(wm8960, WM8960_POWER3, flag));

		flag = WM8960_MIX_D2O | WM_MIX_GAIN(-21);
		WM_ERROR_CHECK(write(wm8960, WM8960_LOUTMIX, flag));
		WM_ERROR_CHECK(write(wm8960, WM8960_ROUTMIX, flag));

		if(wm8960->ctl.output->classD == WM_TRUE){
			flag = (wm8960->ctl.output->channnel << 6);
			WM_ERROR_CHECK(write(wm8960, WM8960_CLASSD1, flag));

		}

		// set volume to -20 dB
		flag = WM_DAC_GAIN(-20) | WM8960_DACVU;
		WM_ERROR_CHECK(write(wm8960, WM8960_LDAC, flag));
		WM_ERROR_CHECK(write(wm8960, WM8960_RDAC, flag));

		// set deemphasis and unmute
		flag = read(wm8960, WM8960_DACCTL1);
		flag |= wm8960->ctl.output->deemph;
		flag &= ~(WM8960_DACMU);
		WM_ERROR_CHECK(write(wm8960, WM8960_DACCTL1, flag));

		if(wm8960->ctl.output->monoMix == WM_TRUE){
			WM_ERROR_CHECK(write(wm8960, WM8960_ADDCTL1, WM8960_MIX_MONO));	
		}

		// set reverb
		if(wm8960->ctl.output->reverb.enable == WM_TRUE){
			flag = (((0x0F) & wm8960->ctl.output->reverb.depth) << 1);

			if(wm8960->ctl.output->reverb.highPass == WM_TRUE)
				flag |= WM8960_3D_HIGHPASS;

			if(wm8960->ctl.output->reverb.lowPass == WM_TRUE)
				flag |= WM8960_3D_LOWPASS;

			WM_ERROR_CHECK(write(wm8960, WM8960_3D, flag));

		}

	}
	
	return WM_OK;
}

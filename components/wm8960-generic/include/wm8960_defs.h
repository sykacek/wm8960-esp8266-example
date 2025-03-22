#ifndef WM8960_DEFINES
#define WM8960_DEFINES

/**
 * WM8960 Register Defines
 */

/* SYSCLK 12.288 MHz for 48kHz */
/* SYSCLK 11.2896 MHz for 44.1kHz*/

#define WM8960_CACHEREGNUM 	56

#define WM8960_LINVOL		0x0
#define WM8960_RINVOL		0x1
#define WM8960_LOUT1		0x2
#define WM8960_ROUT1		0x3
#define WM8960_CLOCK1		0x4
#define WM8960_DACCTL1		0x5
#define WM8960_DACCTL2		0x6
#define WM8960_IFACE1		0x7
#define WM8960_CLOCK2		0x8
#define WM8960_IFACE2		0x9
#define WM8960_LDAC		0xa
#define WM8960_RDAC		0xb

#define WM8960_RESET		0xf
#define WM8960_3D		0x10
#define WM8960_ALC1		0x11
#define WM8960_ALC2		0x12
#define WM8960_ALC3		0x13
#define WM8960_NOISEG		0x14
#define WM8960_LADC		0x15
#define WM8960_RADC		0x16
#define WM8960_ADDCTL1		0x17
#define WM8960_ADDCTL2		0x18
#define WM8960_POWER1		0x19
#define WM8960_POWER2		0x1a
#define WM8960_ADDCTL3		0x1b
#define WM8960_APOP1		0x1c
#define WM8960_APOP2		0x1d

#define WM8960_LINPATH		0x20
#define WM8960_RINPATH		0x21
#define WM8960_LOUTMIX		0x22

#define WM8960_ROUTMIX		0x25
#define WM8960_MONOMIX1		0x26
#define WM8960_MONOMIX2		0x27
#define WM8960_LOUT2		0x28
#define WM8960_ROUT2		0x29
#define WM8960_MONO		0x2a
#define WM8960_INBMIX1		0x2b
#define WM8960_INBMIX2		0x2c
#define WM8960_BYPASS1		0x2d
#define WM8960_BYPASS2		0x2e
#define WM8960_POWER3		0x2f
#define WM8960_ADDCTL4		0x30
#define WM8960_CLASSD1		0x31

#define WM8960_CLASSD3		0x33
#define WM8960_PLL1		0x34
#define WM8960_PLL2		0x35
#define WM8960_PLL3		0x36
#define WM8960_PLL4		0x37


/*
 * WM8960 Clock dividers
 */
#define WM8960_SYSCLKDIV 		0
#define WM8960_DACDIV			1
#define WM8960_OPCLKDIV			2
#define WM8960_DCLKDIV			3
#define WM8960_TOCLKSEL			4

#define WM8960_SYSCLK_AUTO		(2 << 0)

/* WM8960_CLOCK1		0x04*/
#define WM8960_SYSCLK_MCLK		(0x00)
#define WM8960_SYSCLK_PLL		(0x01)
#define WM8960_SYSCLK_DIV_1		(0x00)
#define WM8960_SYSCLK_DIV_2		(0x04)

#define WM8960_DAC_DIV_1		(0x00)
#define WM8960_DAC_DIV_1_5		(0x08)
#define WM8960_DAC_DIV_2		(0x10)
#define WM8960_DAC_DIV_3		(0x18)
#define WM8960_DAC_DIV_4		(0x20)
#define WM8960_DAC_DIV_5_5		(0x28)
#define WM8960_DAC_DIV_6		(0x30)

#define WM8960_ADC_DIV_1		(0x00)
#define WM8960_ADC_DIV_1_5		(0x40)
#define WM8960_ADC_DIV_2		(0x80)
#define WM8960_ADC_DIV_3		(0xc0)
#define WM8960_ADC_DIV_4		(0x100)
#define WM8960_ADC_DIV_5_5		(0x140)
#define WM8960_ADC_DIV_6		(0x180)

#define WM8960_CLOCK1_DEFAULT		(WM8960_SYSCLK_PLL | WM8960_SYSCLK_DIV_2)

/* WM8960_CLOCK2		0x08*/

#define WM8960_BCLK_DIV_1		(0x00)
#define WM8960_BCLK_DIV_1_5		(0x01)
#define WM8960_BCLK_DIV_2		(0x02)
#define WM8960_BCLK_DIV_3		(0x03)
#define WM8960_BCLK_DIV_4		(0x04)
#define WM8960_BCLK_DIV_5_5		(0x05)
#define WM8960_BCLK_DIV_6		(0x06)
#define WM8960_BCLK_DIV_8		(0x07)
#define WM8960_BCLK_DIV_11		(0x08)
#define WM8960_BCLK_DIV_12		(0x09)
#define WM8960_BCLK_DIV_16		(0x0a)
#define WM8960_BCLK_DIV_22		(0x0b)
#define WM8960_BCLK_DIV_24		(0x0c)
#define WM8960_BCLK_DIV_32		(0x0d)

#define WM8960_DCLK_DIV_1_5		(0x00)
#define WM8960_DCLK_DIV_2		(0x40)
#define WM8960_DCLK_DIV_3		(0x80)
#define WM8960_DCLK_DIV_4		(0xc0)
#define WM8960_DCLK_DIV_6		(0x100)
#define WM8960_DCLK_DIV_8		(0x140)
#define WM8960_DCLK_DIV_12		(0x180)
#define WM8960_DCLK_DIV_16		(0xc0)

#define WM8960_CLOCK2_DEFAULT		(WM8960_BCLK_DIV_4 | WM8960_DCLK_DIV_16)

/* WM8960_PLL1		0x34 */
#define WM8960_PLL_DIV_2		(0x10)
#define WM8960_PLL_DIV_1		(0x00)

#define WM8960_PLL_INTEGER		(0x00)
#define WM8960_PLL_FRACTION		(0x20)

#define WM8960_OPCLK_DIV_1		(0x00)
#define WM8960_OPCLK_DIV_2		(0x40)
#define WM8960_OPCLK_DIV_3		(0x80)
#define WM8960_OPCLK_DIV_4		(0xc0)
#define WM8960_OPCLK_DIV_5_5		(0x100)
#define WM8960_OPCLK_DIV_6		(0x140)

#define PLL				0x3126E8

/* 2 - Wire Communication Protocol */
#define I2S_BIT_WRITE			(0x0)
#define I2S_BIT_READ			(0x1)

/* Power Managament flags */
//WM8960_POWER1		0x19
#define WM8960_MASTER_CLOCK		(0x01)
#define WM8960_PWR_MIC_BIAS		(0x02)
#define WM8960_PWR_ADCR			(0x04)
#define WM8960_PWR_ADCL			(0x08)
#define WM8960_PWR_AINR			(0x10)
#define WM8960_PWR_AINL			(0x20)
#define WM8960_VREF			(0x40)
#define WM8960_VMID_NORMAL		(0x80)		//2x 50k ohm
#define WM8960_VMID_STANDBY		(0x100)		//2x 250k ohm
#define WM8960_VMID_STARTUP		(0x180)		//2x 5k ohm

#define WM8960_POWER1_DEFAULT		(WM8960_VMID_NORMAL | WM8960_VREF)

//WM8960_POWER2		0x1a
#define WM8960_PWR_PLL			(0x01)
#define WM8960_PWR_OUT3			(0x02)
#define WM8960_PWR_SPKR			(0x08)
#define WM8960_PWR_SPKL			(0x10)
#define WM8960_PWR_ROUT1		(0x20)
#define WM8960_PWR_LOUT1		(0x40)
#define WM8960_PWR_DACR			(0x80)
#define WM8960_PWR_DACL			(0x100)

#define WM8960_POWER2_DEFAULT		(WM8960_PWR_PLL | WM8960_PWR_DACR | \
					WM8960_PWR_DACL | WM8960_PWR_LOUT1 | \
					WM8960_PWR_ROUT1)


//WM8960_POWER3		0x2f
#define WM8960_PWR_RMIX			(0x04)
#define WM8960_PWR_LMIX			(0x08)
#define WM8960_PWR_RMIC			(0x10)
#define WM8960_PWR_LMIC			(0x20)

#define WM8960_POWER3_DEFAULT_IN	(WM8960_PWR_RMIC | WM8960_PWR_LMIC)
#define WM8960_POWER3_DEFAULT_OUT	(WM8960_PWR_LMIX | WM8960_PWR_RMIX)

/* WM8960_LINVOL1, RINVOL2	0x01, 0x02*/
#define WM8960_PGA_ZERO_CROSS		(0x40)
#define WM8960_PGA_MUTE			(0x80)
#define WM8960_PGA_VU			(0x100)

/* range from -17.25 dB to 30 dB */
#define WM_PGA_GAIN(dB)			((uint8_t)(dB / 0.75f) + 23)

/* WM8960_LOUT1, ROUT1		0x02, 0x03 */
#define WM8960_OUT_ZERO_CROSS		(0x80)
#define WM8960_HEADPHONE_VU		(0x100)

/* range from 6 dB to -73 dB */
#define WM_OUT_GAIN(dB)			((uint8_t)((121 + dB) & 0x7f))

// DAC Volume register 0xA 0xB
#define WM_DAC_GAIN(dB)			((uint8_t)((2 * dB + 255) & 0xff))
#define WM8960_DAC_VU			(0x100)

// Left + Right Mixer 0x22, 0x25
#define WM8960_MIX_0dB			(0x00)
#define WM8960_MIX_NEG_3dB		(0x10)
#define WM8960_MIX_NEG_6dB		(0x20)
#define WM8960_MIX_NEG_9dB		(0x30)
#define WM8960_MIX_NEG_12dB		(0x40)
#define WM8960_MIX_NEG_15dB		(0x50)
#define WM8960_MIX_NEG_18dB		(0x60)
#define WM8960_MIX_NEG_21dB		(0x70)

#define WM_INPUT2MIX_GAIN(dB)		((uint8_t) (dB / (-3)))

#define WM8960_MIX_I2O			(0x80)	// input 3 to mixer
#define WM8960_MIX_D2O			(0x100)	// dac to mixer 

// WM8960_ADDCTL1 		0x17
#define WM8960_TIMEOUT			(0x01)
#define WM8960_TIMEOUT_CLK		(0x20)
#define WM8960_MIX_MONO			(0x10)

// 0x26, 0x27
#define WM8960_2MO			(0x80)

// ADCDACCTL1 0x05
#define WM8960_ADCHPD			(0x01)	//ADC highpass filter
/* deemphasis enums */
#define WM8960_DACMU			(0x08)
/* polarity enums */
#define WM8960_DAC_ATTEN		(0x80)	//6 dB attenuate

// ADCDACCTL2 0x06
#define WM8960_DACSLOPE			(0x02)
#define WM8960_DACMR			(0x04)	//DAC mute rate
#define WM8960_DACSMM			(0x08)	//DAC soft mute mode
/* polarity enums */

// WM8960_IFACE1		0x07
/* format enums */
/* word lengthenums */
#define WM8960_LRP			(0x10)	//LRCLK polarity
#define WM8960_DACSWAP			(0x20)
#define WM8960_MASTER			(0x40)
#define WM8960_BCLKINV			(0x80)
#define WM8960_ADCSWAP			(0x100)

// WM8960_IFACE2		0x09
#define WM8960_LOOPBACK			(0x01)
#define WM8960_ALRCK_GPIO		(0x40)


// WM8960_3D 		3D enhacement
#define WM8960_3D_HIGHPASS		(0x20)
#define WM8960_3D_LOWPASS		(0x40)

/* WM8960_CLASSD2		0x33 */
#define WM8960_DCGAIN_MAX		(0x24)
#define WM8960_ACGAIN_MAX		(0x05)

/* WM8960_BYPASS1 + WM8960_BYPASS2	0x2d, 0x2e */
#define WM8960_BOOST2MIX		(0x80)
#define WM_MIX_GAIN(dB)			((uint8_t)((dB / -3) << 4))

#define WM8960_MEMORY_RESERVED		(0xFFFF)
#define WM8960_MEMORY_DEFAULT		(regmap_t){ \
					.reg[0x0] = 0x0097, \
					.reg[0x1] = 0x0097, \
					.reg[0x2] = 0x0000, \
					.reg[0x3] = 0x0000, \
					.reg[0x4] = 0x0000, \
					.reg[0x5] = 0x0008, \
					.reg[0x6] = 0x0000, \
					.reg[0x7] = 0x000a, \
					.reg[0x8] = 0x01c0, \
					.reg[0x9] = 0x0000, \
					.reg[0xa] = 0x00ff, \
					.reg[0xb] = 0x00ff, \
					.reg[0xc] = WM8960_MEMORY_RESERVED, \
					.reg[0xd] = WM8960_MEMORY_RESERVED, \
					.reg[0xe] = WM8960_MEMORY_RESERVED, \
					.reg[0xf] = WM8960_MEMORY_RESERVED, \
					.reg[0x10] = 0x0000, \
					.reg[0x11] = 0x007b, \
					.reg[0x12] = 0x0100, \
					.reg[0x13] = 0x0032, \
					.reg[0x14] = 0x0000, \
					.reg[0x15] = 0x00c3, \
					.reg[0x16] = 0x00c3, \
					.reg[0x17] = 0x01c0, \
					.reg[0x18] = 0x0000, \
					.reg[0x19] = 0x0000, \
					.reg[0x1a] = 0x0000, \
					.reg[0x1b] = 0x0000, \
					.reg[0x1c] = 0x0000, \
					.reg[0x1d] = 0x0000, \
					.reg[0x1e] = WM8960_MEMORY_RESERVED, \
					.reg[0x1f] = WM8960_MEMORY_RESERVED, \
					.reg[0x20] = 0x0000, \
					.reg[0x21] = 0x0100, \
					.reg[0x22] = 0x0050, \
					.reg[0x23] = WM8960_MEMORY_RESERVED, \
					.reg[0x24] = WM8960_MEMORY_RESERVED, \
					.reg[0x25] = 0x0050, \
					.reg[0x26] = 0x0000, \
					.reg[0x27] = 0x0000, \
					.reg[0x28] = 0x0000, \
					.reg[0x29] = 0x0000, \
					.reg[0x2a] = 0x0040, \
					.reg[0x2b] = 0x0000, \
					.reg[0x2c] = 0x0000, \
					.reg[0x2d] = 0x0050, \
					.reg[0x2e] = 0x0050, \
					.reg[0x2f] = 0x0000, \
					.reg[0x30] = 0x0002, \
					.reg[0x31] = 0x0037, \
					.reg[0x32] = WM8960_MEMORY_RESERVED, \
					.reg[0x33] = 0x0080, \
					.reg[0x34] = 0x0008, \
					.reg[0x35] = 0x0031, \
					.reg[0x36] = 0x0026, \
					.reg[0x37] = 0x00e9, }

#endif
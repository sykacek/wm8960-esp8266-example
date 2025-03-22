#ifndef WM8960_H
#define WM8960_H

#include "wm8960_defs.h"

#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define WM_ERROR_CHECK(func)		if(func) { return WM_ERROR_GENERIC; }

/**
 * @brief uses only input 1, jack is connected to speaker
 * 
 */

typedef enum WM_STATUS {
	WM_OK = 0,
	WM_ERROR_GENERIC = 1,
	WM_ERROR_INVALID_PARAM = 2,
	WM_ERROR_NULLPTR = 3,
	WM_ERROR_NOT_PERMITTED = 4,
} WM_STATUS;

typedef enum WM_CHANNEL {
	WM_CHANNEL_LEFT = 0x1,
	WM_CHANNEL_RIGHT = 0x2,
	WM_CHANNEL_BOTH	= 0x4,
} WM_CHANNEL;

typedef enum WM_BOOL {
	WM_TRUE = 1,
	WM_FALSE = 0,
} WM_BOOL;

// deephasis bit flags for register 0x5
typedef enum WM_DEEMPH {
	WM_DEEMPH_48K = 0x6,
	WM_DEEMPH_41K = 0x4,
	WM_DEEMPH_32K = 0x2,
	WM_DEEMPH_NONE = 0x0,
} WM_DEEMPH;

// enumerations for inverting dac channels polarity 
typedef enum WM_POLARITY {
	WM_INVERT_L = 0x20,
	WM_INVERT_R = 0x40,
	WM_INVERT_LR = 0x60,
	WM_POLARITY_NORMAL = 0x0,
} WM_POLARITY;

typedef enum WM_WORD_LENGTH {
	WM_WORD_16BIT = 0x00,
	WM_WORD_20BIT = 0x04,
	WM_WORD_24BIT = 0x08,
	WM_WORD_32BIT = 0x0c,
} WM_WORD_LENGTH;

typedef enum WM_DATA_FMT {
	WM_FMT_RIGHT = 0x0,
	WM_FMT_LEFT = 0x01,
	WM_FMT_I2S = 0x02,
	WM_FMT_DSP = 0x03,
} WM_DATA_FMT;

typedef enum WM_COMPAND {
	WM_COMPAND_MICRO = 0x2,
	WM_COMPAND_A = 0x3,
	WM_COMPAND_NONE = 0x0,
} WM_COMPAND;

/* struct for storing memory copy since wm8960 does not support reading registers
* includes reserved registers
*/
typedef struct regmap {
	uint16_t reg[0x38]; 
} regmap_t;

typedef struct reverb {
	WM_BOOL enable;
	WM_BOOL highPass;
	WM_BOOL lowPass;
	uint8_t depth;
} reverb_t;

typedef struct wm8960_input_ctl {
	WM_CHANNEL channnel;
	WM_POLARITY polarity;
	WM_BOOL noiseGate;
	uint8_t pga;
	WM_BOOL acl;		//automatic level control
	WM_COMPAND compander;
} wm8960_input_ctl_t;

typedef struct wm8960_output_ctl {
	WM_CHANNEL channnel;
	WM_BOOL headphone;
	WM_BOOL classD;
	WM_DEEMPH deemph;
	WM_POLARITY polarity;
	WM_BOOL monoMix;
	reverb_t reverb;
	WM_COMPAND compander;
} wm8960_output_ctl_t;

typedef struct wm8960_ctl {
	uint8_t addr;			// 2 - wire address (0x34 default) (0x34 >> 1 = 0x1a) 
	wm8960_input_ctl_t *input;
	wm8960_output_ctl_t *output;
	WM_DATA_FMT fmt;
	WM_WORD_LENGTH word;
} wm8960_ctl_t;

typedef struct wm8960 {
	wm8960_ctl_t ctl;		//control struct
	regmap_t memory;
	/* specify function to delay in ms */
	void (*delay_ms)(uint32_t __ms);

	/* write byte of data to reg*/
	WM_STATUS (*write_reg)(uint8_t addr, uint8_t reg, uint16_t data);
} wm8960_t;

/**
 * @brief initialize wm8960 
 * 
 * @param wm8960 pointer to wm8960 base struct
 * @return WM_STATUS -	WM_OK on success
			WM_INVALID_PARAM if parameters are bad 
 */
WM_STATUS WMinit(wm8960_t *wm8960);

/**
 * @brief update running instance of wm8960
 * 
 * @param wm8960 pointer to wm8960 base struct
 * @return WM_STATUS 
 */
WM_STATUS WMupdate(wm8960_ctl_t *wm8960);

WM_STATUS WMreset(wm8960_t *wm8960);

WM_STATUS WMsetPLL(wm8960_t *wm8960, uint8_t integerPart, uint32_t fractionalPart);

WM_STATUS WMsetMixVol(wm8960_t *wm8960, uint16_t vol);

WM_STATUS WMsetDACVol(wm8960_t *wm8960, uint8_t vol);

WM_STATUS WMsetMixBoost(wm8960_t *wm8960, uint8_t boost);
WM_STATUS WMsetDACaAtten(wm8960_t *wm8960);

/**
 * @brief read data from channel
 * 
 * @param wm8960 
 * @param channel 
 * @param data 
 * @return WM_STATUS 
 */
WM_STATUS WMreadChannel(wm8960_t *wm8960, uint8_t *data);

/**
 * @brief mute selected channel
 * 
 */
WM_STATUS WMmuteOutputChannel(wm8960_t *wm8960, WM_CHANNEL channel);

/**
 * @brief set volume of selected channel
 * 
 */
WM_STATUS WMsetInputMicGain(wm8960_t *wm8960, WM_CHANNEL channel, uint8_t gain);

WM_STATUS WMsetInputLineGain(wm8960_t *wm8960, WM_CHANNEL channel, uint8_t gain);

WM_STATUS WMsetNoiseGateTreshold(wm8960_t *wm8960, uint8_t threshold);

#endif
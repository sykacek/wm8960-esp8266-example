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
	WM_DACL_EN = (1 << 5),
	WM_DACR_EN = (1 << 6),
	WM_DACLR_EN = (1 << 5) | (1 << 6),
	WM_DACLR_DIS = 0x0,
} WM_POLARITY;

/* struct for storing memory copy since wm8960 does not support reading registers
*includes reserved registers
*/
typedef struct regmap {
	uint16_t reg[0x38]; 
} regmap_t;

typedef struct reverb {
	WM_BOOL highPass;
	WM_BOOL lowPass;
	uint8_t depth;
} reverb_t;

typedef struct wm8960_input_ctl {
	WM_CHANNEL channnel;
	WM_BOOL noiseGate;
	uint8_t pgaLeft;
	uint8_t pgaRight;
	WM_BOOL acl;		//automatic level control
} wm8960_input_ctl_t;

typedef struct wm8960_output_ctl {
	WM_CHANNEL channnel;
	WM_BOOL speaker;
	WM_BOOL headphone;
	WM_BOOL classD;
	uint8_t reverb;
	WM_DEEMPH deemph;
	WM_BOOL monoMix;
	reverb_t *reverb;
	int frequency;
} wm8960_output_ctl_t;

typedef struct wm8960_ctl {
	uint8_t addr;			// 2 - wire address (0x34 default) 
	wm8960_input_ctl_t *input;
	wm8960_output_ctl_t *output;
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

WM_STATUS WMmanagePower(wm8960_t *wm8960, int32_t powerMode);

/**
 * @brief read data from channel
 * 
 * @param wm8960 
 * @param channel 
 * @param data 
 * @return WM_STATUS 
 */
WM_STATUS WMreadChannel(wm8960_t *wm8960, WM_CHANNEL channel, uint8_t *data);

/**
 * @brief mute selected channel
 * 
 */
WM_STATUS WMmuteOutputChannel(wm8960_t *wm8960, WM_CHANNEL channel);

/**
 * @brief set volume of selected channel
 * 
 */
WM_STATUS WMsetOutputVolume(wm8960_t *wm8960, WM_CHANNEL channel, uint8_t volume);
WM_STATUS WMsetOutputMixerBoost(wm8960_t *wm8960, WM_CHANNEL channel, uint8_t boost);

WM_STATUS WMsetInputMicGain(wm8960_t *wm8960, WM_CHANNEL channel, uint8_t gain);

WM_STATUS WMsetInputLineGain(wm8960_t *wm8960, WM_CHANNEL channel, uint8_t gain);

WM_STATUS WMsetNoiseGateTreshold(wm8960_t *wm8960, uint8_t threshold);

#endif
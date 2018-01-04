/*
 * HT12D.h
 *
 *  Created on: 3 ñ³÷. 2018 ð.
 *      Author: Bogdan
 */

#ifndef HT12D_H_
#define HT12D_H_

#include "ch.h"
#include "hal.h"
#include <string.h>

class HT12D {
#define FOSC_ENCODER	(2500)
#define GPT_FREQ	(2500*80)

	typedef void (*callback_t)(uint16_t data);

	GPTDriver* _gptd;
	callback_t _callback;
	ioportid_t _port;
	uint8_t _pad;

	enum {
		kWait,
		kSyncPulse,
		kData,
	} state = kWait;

	uint16_t pilot_timer = 0;
	uint16_t sync_timer = 0;

	uint16_t catch_time = 0;
	uint16_t timer = 0;
	uint16_t filtered_timer = 0;
	uint16_t data = 0;
	uint8_t data_index = 0;

	uint16_t buffer[4] = {0xFFFF};

public:
	HT12D(ioportid_t port, uint8_t pad, callback_t callback, GPTDriver* gptd);
	virtual ~HT12D();

	void extcb_gpioa_0(EXTDriver *extp, expchannel_t channel);
	void gpt_cb(GPTDriver *gptp);
};

#endif /* HT12D_H_ */

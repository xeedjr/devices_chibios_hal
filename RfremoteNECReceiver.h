/*
 * RfremoteNECReceiver.h
 *
 *  Created on: 2 ñ³÷. 2018 ð.
 *      Author: Bogdan
 */

#ifndef RFREMOTENECRECEIVER_H_
#define RFREMOTENECRECEIVER_H_

#include "ch.h"
#include "hal.h"
#include <string.h>


class RfremoteNECReceiver {
	typedef void (*callback_t)(uint8_t address, uint8_t command);

	callback_t _callback;
	ioportid_t _port;
	uint8_t _pad;

	enum {
		kWait = 0,
		kLeadingPulse,
		kLeadingPulseSpace,
		kBurst,
		kBurstSpace
	} state = kWait;
	systime_t start = 0;
	uint8_t byte = 0;
	uint8_t address_command[4] = {0};
	uint8_t address_command_index = 0;
	uint8_t bits = 0;

	void set_bit(uint8_t bit);

public:
	RfremoteNECReceiver(ioportid_t port, uint8_t pad, callback_t callback);
	virtual ~RfremoteNECReceiver();

	void extcb_gpioa_0(EXTDriver *extp, expchannel_t channel);
};

#endif /* RFREMOTENECRECEIVER_H_ */

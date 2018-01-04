/*
 * RfReceiverPT2262.h
 *
 *  Created on: 2 ñ³÷. 2018 ð.
 *      Author: Bogdan
 */

#ifndef RfReceiverPT2262_H_
#define RfReceiverPT2262_H_

#include "ch.h"
#include "hal.h"
#include <string.h>

class RfReceiverPT2262 {
	ioportid_t _port;
	uint8_t _pad;
#define PERIOD	13
	enum {
		kWait = 0,
		kHi,
		kLow
	} state = kWait;

	uint16_t address_data = 0;
	uint8_t bits_index = 0;
	uint16_t frame[4];
	uint8_t frame_index = 0;

	systime_t start = 0;
	uint8_t byte = 0;
	uint8_t address_command[4] = {0};
	uint8_t address_command_index = 0;
	uint8_t bits = 0;

	void set_bit(uint8_t bit);

public:
	RfReceiverPT2262(ioportid_t port, uint8_t pad);
	virtual ~RfReceiverPT2262();

	void extcb_gpioa_0(EXTDriver *extp, expchannel_t channel);
};

#endif /* RfReceiverPT2262_H_ */

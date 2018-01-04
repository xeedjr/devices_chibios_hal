/*
 * RfLinkNEC.cpp
 *
 * Created: 30.12.2017 15:41:40
 *  Author: Bogdan
 */ 

#include "hal.h"

#if HAL_USE_PAL

#include "RfLinkPT2262.h"
#include "cmsis_os.h"

void RfLinkPT2262::init(ioportid_t port, uint8_t pad) {
	_port = port;
	_pad = pad;
}

void RfLinkPT2262::LedHi() {
	palSetPad(_port, _pad);
}

void RfLinkPT2262::LedLo() {
	palClearPad(_port, _pad);
}

void RfLinkPT2262::SendBit(bool bit) {
	LedHi();
	if (bit) {
		chThdSleepMicroseconds(PERIOD*12);
		LedLo();
		chThdSleepMicroseconds(PERIOD*4);
	} else {
		chThdSleepMicroseconds(PERIOD*4);
		LedLo();
		chThdSleepMicroseconds(PERIOD*12);
	};
}

void RfLinkPT2262::SendSync() {
	LedHi();
	chThdSleepMicroseconds(PERIOD*4);
	LedLo();
	chThdSleepMicroseconds(PERIOD*128);
}

void RfLinkPT2262::SendWord(uint16_t addr_data) {
	SendBit(addr_data & (1 << 0));
	SendBit(addr_data & (1 << 1));
	SendBit(addr_data & (1 << 2));
	SendBit(addr_data & (1 << 3));
	SendBit(addr_data & (1 << 4));
	SendBit(addr_data & (1 << 5));
	SendBit(addr_data & (1 << 6));
	SendBit(addr_data & (1 << 7));
	SendBit(addr_data & (1 << 8));
	SendBit(addr_data & (1 << 9));
	SendBit(addr_data & (1 << 10));
	SendBit(addr_data & (1 << 11));
	SendSync();
}

void RfLinkPT2262::SendFrame(uint16_t addr_data) {
	SendWord(addr_data);
	SendWord(addr_data);
	SendWord(addr_data);
	SendWord(addr_data);
}
#endif /* HAL_USE_PAL */
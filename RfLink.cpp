/*
 * RfLink.cpp
 *
 * Created: 30.12.2017 15:41:40
 *  Author: Bogdan
 */ 

#include "hal.h"

#if HAL_USE_PAL

#include "RfLink.h"
#include "cmsis_os.h"

void RfLink::init(ioportid_t port, uint8_t pad) {
	_port = port;
	_pad = pad;
}

void RfLink::LedHi() {
	palSetPad(_port, _pad);
}

void RfLink::LedLo() {
	palClearPad(_port, _pad);
}

void RfLink::SendBit(bool bit) {
	if (bit) {
		LedLo();
		chThdSleepMicroseconds(200);
		LedHi();
		chThdSleepMicroseconds(200);
	} else {
		LedHi();
		chThdSleepMicroseconds(200);
		LedLo();
		chThdSleepMicroseconds(200);
	};
}

void RfLink::SendByte(uint8_t byte) {
	SendBit(byte & (1 << 0));
	SendBit(byte & (1 << 1));
	SendBit(byte & (1 << 2));
	SendBit(byte & (1 << 3));
	SendBit(byte & (1 << 4));
	SendBit(byte & (1 << 5));
	SendBit(byte & (1 << 6));
	SendBit(byte & (1 << 7));
}

void RfLink::SendCommand(uint8_t addr, uint8_t command) {
	SendByte(0xAA);
	SendByte(0xAA);
	SendByte(0xCD);
	SendByte(addr);
	SendByte(~addr);
	SendByte(command);
	SendByte(~command);
}

#endif /* HAL_USE_PAL */
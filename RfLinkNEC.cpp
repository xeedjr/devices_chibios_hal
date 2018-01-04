/*
 * RfLinkNEC.cpp
 *
 * Created: 30.12.2017 15:41:40
 *  Author: Bogdan
 */ 

#include "hal.h"

#if HAL_USE_PAL

#include "RfLinkNEC.h"
#include "cmsis_os.h"

void RfLinkNEC::init(ioportid_t port, uint8_t pad) {
	_port = port;
	_pad = pad;
}

void RfLinkNEC::LedHi() {
	palSetPad(_port, _pad);
}

void RfLinkNEC::LedLo() {
	palClearPad(_port, _pad);
}

void RfLinkNEC::SendBit(bool bit) {
	LedHi();
	chThdSleepMicroseconds(560);
	LedLo();
	if (bit)
		chThdSleepMicroseconds(1690);
	else
		chThdSleepMicroseconds(560);
}

void RfLinkNEC::SendPreamble() {
	LedHi();
	chThdSleepMilliseconds(9);
	LedLo();
	chThdSleepMicroseconds(4500);
}

void RfLinkNEC::SendEndBurst() {
	LedHi();
	chThdSleepMicroseconds(560);
	LedLo();
}

void RfLinkNEC::SendByte(uint8_t byte) {
	SendBit(byte & (1 << 0));
	SendBit(byte & (1 << 1));
	SendBit(byte & (1 << 2));
	SendBit(byte & (1 << 3));
	SendBit(byte & (1 << 4));
	SendBit(byte & (1 << 5));
	SendBit(byte & (1 << 6));
	SendBit(byte & (1 << 7));
}

void RfLinkNEC::SendCommand(uint8_t addr, uint8_t command) {
	SendBit(1);
	SendBit(0);
	SendPreamble();
	SendByte(addr);
	SendByte(~addr);
	SendByte(command);
	SendByte(~command);
	SendEndBurst();
}

#endif /* HAL_USE_PAL */
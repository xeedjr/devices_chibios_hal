/*
 * HT12E.cpp
 *
 * Created: 30.12.2017 15:41:40
 *  Author: Bogdan
 */ 

#include "hal.h"

#ifdef PORT_ARCHITECTURE_AVR
#if HAL_USE_PAL

#include "HT12E.h"
#include "cmsis_os.h"
#include <util/delay.h>

void HT12E::init(ioportid_t port, uint8_t pad) {
	_port = port;
	_pad = pad;
}

void HT12E::LedHi() {
	palSetPad(_port, _pad);
}

void HT12E::LedLo() {
	palClearPad(_port, _pad);
}

void HT12E::SendBit(bool bit) {
	LedLo();
	if (bit) {
		_delay_us(BIT_1_LOW_PERIOD);
		LedHi();
		_delay_us(BIT_1_HI_PERIOD);
	} else {
		_delay_us(BIT_0_LOW_PERIOD);
		LedHi();
		_delay_us(BIT_0_HI_PERIOD);
	};
	LedLo();
}

void HT12E::SendSync() {
	_delay_us(PILOT_PERIOD);
	LedHi();
	_delay_us(SYNC_PERIOD);
	LedLo();
}

void HT12E::SendWord(uint16_t addr_data) {
	SendSync();
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
}

void HT12E::SendFrame(uint16_t addr_data) {
	SendWord(addr_data);
	SendWord(addr_data);
	SendWord(addr_data);
	SendWord(addr_data);
}
#endif /* HAL_USE_PAL */
#endif /* PORT_ARCHITECTURE_AVR */

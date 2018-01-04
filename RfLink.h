/*
 * RfLink.h
 *
 * Created: 30.12.2017 22:45:58
 *  Author: Bogdan
 */ 


#ifndef RfLink_H_
#define RfLink_H_

#include "hal.h"

#if HAL_USE_PAL

class RfLink {
	ioportid_t _port;
	uint8_t _pad;
public:
	void init(ioportid_t port, uint8_t pad);
	void LedHi();
	void LedLo();
	void SendBit(bool bit);
	void SendPreamble();
	void SendEndBurst();
	void SendByte(uint8_t byte);
	void SendCommand(uint8_t addr, uint8_t command);
};

#endif /* HAL_USE_PAL */

#endif /* RfLink_H_ */
/*
 * RfLinkPT2262.h
 *
 * Created: 30.12.2017 22:45:58
 *  Author: Bogdan
 */ 


#ifndef RFLINKPT2262_H_
#define RFLINKPT2262_H_

#include "hal.h"

#if HAL_USE_PAL

class RfLinkPT2262 {
#define	PERIOD	13
	ioportid_t _port;
	uint8_t _pad;
public:
	void init(ioportid_t port, uint8_t pad);
	void LedHi();
	void LedLo();
	void SendBit(bool bit);
	void SendSync();
	void SendWord(uint16_t addr_data);
	void SendFrame(uint16_t addr_data);
};

#endif /* HAL_USE_PAL */

#endif /* RFLINKPT2262_H_ */
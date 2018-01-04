/*
 * HT12E.h
 *
 * Created: 30.12.2017 22:45:58
 *  Author: Bogdan
 */ 


#ifndef HT12E_H_
#define HT12E_H_

#include "hal.h"

#if HAL_USE_PAL

class HT12E {
	#define FOSC	(2500)
	#define	PERIOD_T_US	(1000000/FOSC)
	#define BIT_PERIOD (PERIOD_T_US*3)
	#define PILOT_PERIOD (PERIOD_T_US*12)
	#define SYNC_PERIOD (PERIOD_T_US*1)
	#define BIT_1_HI_PERIOD (PERIOD_T_US*1)
	#define BIT_1_LOW_PERIOD (PERIOD_T_US*2)
	#define BIT_0_HI_PERIOD (PERIOD_T_US*2)
	#define BIT_0_LOW_PERIOD (PERIOD_T_US*1)
	
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

#endif /* HT12E_H_ */
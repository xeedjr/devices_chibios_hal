/*
 * LEDBlinker.h
 *
 * Created: 05.01.2018 23:41:33
 *  Author: Bogdan
 */ 


#ifndef LEDBLINKER_H_
#define LEDBLINKER_H_

#include "hal.h"

class LEDBlinker {
	virtual_timer_t vt;
	ioportid_t _port;
	uint8_t _pad;
	uint16_t _pause_ms;
	uint16_t _light_ms;

	void set_timer(uint32_t timeout_ms);
public:
	LEDBlinker();
	~LEDBlinker();
	void init(ioportid_t port,
				uint8_t pad,
				uint16_t pause_ms,
				uint16_t light_ms);
	void led_cb();
};



#endif /* LEDBLINKER_H_ */

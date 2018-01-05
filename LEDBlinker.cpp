/*
 * LEDBlinker.cpp
 *
 * Created: 05.01.2018 23:41:15
 *  Author: Bogdan
 */ 

#include "LEDBlinker.h"

LEDBlinker::LEDBlinker() {

}

LEDBlinker::~LEDBlinker() {
	
}

void LEDBlinker::init(ioportid_t port,
						uint8_t pad) {
	_port = port;
	_pad = pad;
							
	chVTObjectInit(&vt);
	set_timer(500);
}

void LEDBlinker::set_timer(uint32_t timeout_ms) {
	chVTSet(&vt, MS2ST(timeout_ms), [](void *arg){
									((LEDBlinker*)(arg))->led_cb();
								}, this);
}

void LEDBlinker::led_cb() {
	chSysLockFromISR();
	static bool toggle = false;
	if (toggle) {
		palSetPad(_port, _pad);
		toggle = false;
		set_timer(10);
	} else {
		palClearPad(_port, _pad);
		toggle = true;
		set_timer(2000);
	};
	chSysUnlockFromISR();
}


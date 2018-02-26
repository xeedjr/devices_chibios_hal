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
						uint8_t pad,
						uint16_t pause_ms,
						uint16_t light_ms) {
	_port = port;
	_pad = pad;
	_pause_ms = pause_ms;
	_light_ms = light_ms;
							
	chVTObjectInit(&vt);
	set_timer(500);
}

void LEDBlinker::set_timer(uint32_t timeout_ms) {
	if (port_is_isr_context()) {
		chVTSetI(&vt, MS2ST(timeout_ms), [](void *arg){
										((LEDBlinker*)(arg))->led_cb();
									}, this);
	} else {
		chVTSet(&vt, MS2ST(timeout_ms), [](void *arg){
										((LEDBlinker*)(arg))->led_cb();
									}, this);
	}
}

void LEDBlinker::led_cb() {
	chSysLockFromISR();
	static bool toggle = false;
	if (toggle) {
		palSetPad(_port, _pad);
		toggle = false;
		set_timer(_light_ms);
	} else {
		palClearPad(_port, _pad);
		toggle = true;
		set_timer(_pause_ms);
	};
	chSysUnlockFromISR();
}


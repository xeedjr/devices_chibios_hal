/*
 * SensorButton.cpp
 *
 *  Created on: 06 но€б. 2016 г.
 *      Author: Bogdan
 */

#include "SensorButton.h"

SensorButton::SensorButton(ioportid_t port,
							uint8_t pin,
							callback_t pushed_cb,
							callback_t released_cb) {
	port_ = port;
	pin_ = pin;
	pushed_cb_ = pushed_cb;
	released_cb_ = released_cb;
}

SensorButton::~SensorButton() {
	// TODO Auto-generated destructor stub
}

void SensorButton::event_process_() {
	if (is_pushed_old_state_ == true) {

		if (palReadPad(port_, pin_) == 0) {
			/// un pushed GPIOA_BUTTON
			is_pushed_old_state_ = false;

			if (released_cb_)
			released_cb_();
		};
	} else {
		if (palReadPad(port_, pin_) == 1) {
			/// pushed GPIOA_BUTTON
			/// received new state need send event and change old state
			is_pushed_old_state_ = true;

			if (pushed_cb_)
			pushed_cb_();
		};
	};
}

/** @brief !!! Shall be in  chSysLockFromISR
 *
 */
void SensorButton::event() {
	event_process_();
}

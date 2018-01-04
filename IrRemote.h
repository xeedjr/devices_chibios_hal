/*
 * IrRemote.h
 *
 * Created: 28.12.2017 21:36:52
 *  Author: Bogdan
 */ 


#ifndef IRREMOTE_H_
#define IRREMOTE_H_

#include "hal.h"

#if HAL_USE_PWM

class IrRemote {
	const PWMConfig* _config;
	PWMDriver* _pwm;
	uint8_t _channel;
public:
	void init(PWMDriver* pwm, PWMConfig* cfg, uint8_t channel);
	void LedHi();
	void LedLo();
	void SendBit(bool bit);
	void SendPreamble();
	void SendEndBurst();
	void SendByte(uint8_t byte);
	void SendCommand(uint8_t addr, uint8_t command);
};



#endif /* HAL_USE_PWM */

#endif /* IRREMOTE_H_ */

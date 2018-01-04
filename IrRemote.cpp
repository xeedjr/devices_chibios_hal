/*
 * IrRemote.cpp
 *
 * Created: 28.12.2017 21:36:39
 *  Author: Bogdan
 */ 

#include "hal.h"

#if HAL_USE_PWM

#include "IrRemote.h"
#include "cmsis_os.h"

void IrRemote::init(PWMDriver* pwm, PWMConfig* cfg, uint8_t channel) {
	_pwm = pwm;
	_channel = channel;
	pwmStart(_pwm, cfg);
}

void IrRemote::LedHi() {
	pwmEnableChannel(_pwm, _channel, 210);
}

void IrRemote::LedLo() {
	pwmDisableChannel(_pwm, _channel);
}

void IrRemote::SendBit(bool bit) {
	LedHi();
	chThdSleepMicroseconds(560);
	LedLo();
	if (bit)
		chThdSleepMicroseconds(1690);
	else 
		chThdSleepMicroseconds(560);
}

void IrRemote::SendPreamble() {
	LedHi();
	chThdSleepMilliseconds(9);
	LedLo();
	chThdSleepMicroseconds(4500);
}

void IrRemote::SendEndBurst() {
	LedHi();
	chThdSleepMicroseconds(560);
	LedLo();
}

void IrRemote::SendByte(uint8_t byte) {
	SendBit(byte & (1 << 0));
	SendBit(byte & (1 << 1));
	SendBit(byte & (1 << 2));
	SendBit(byte & (1 << 3));
	SendBit(byte & (1 << 4));
	SendBit(byte & (1 << 5));
	SendBit(byte & (1 << 6));
	SendBit(byte & (1 << 7));
}

void IrRemote::SendCommand(uint8_t addr, uint8_t command) {
	SendPreamble();
	SendByte(addr);
	SendByte(~addr);
	SendByte(command);
	SendByte(~command);
	SendEndBurst();
}

#endif /* HAL_USE_PWM */
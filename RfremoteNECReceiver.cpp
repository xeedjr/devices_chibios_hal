/*
 * RfremoteNECReceiver.cpp
 *
 *  Created on: 2 ñ³÷. 2018 ð.
 *      Author: Bogdan
 */

#include <RfremoteNECReceiver.h>

RfremoteNECReceiver::RfremoteNECReceiver(ioportid_t port, uint8_t pad, callback_t callback) {
	// TODO Auto-generated constructor stub
	_port = port;
	_pad = pad;
	_callback = callback;
}

RfremoteNECReceiver::~RfremoteNECReceiver() {
	// TODO Auto-generated destructor stub
}

void RfremoteNECReceiver::set_bit(uint8_t bit) {
	bit = bit << bits;
	byte |= bit;
	bits++;

	if (bits == 8) {
		address_command[address_command_index++] = byte;
		byte = 0;
		bits = 0;
	}

	if (address_command_index == 4) {
		uint8_t addres = address_command[0];
		uint8_t address_inv = ~address_command[1];
		uint8_t command = address_command[2];
		uint8_t command_inv = ~address_command[3];

		if ((addres == address_inv) &&
			(command == command_inv) ) {
			_callback(addres, command);
		};
		address_command_index = 0;
	}
}

void RfremoteNECReceiver::extcb_gpioa_0(EXTDriver *extp, expchannel_t channel) {
	  (void)extp;
	  (void)channel;
//	  chSysLockFromISR();

	  {
		  bool is_rising = true;

		  if (palReadPad(_port, _pad) == 1) {
			  /// Hi
			  is_rising = true;
		  } else {
			  /// Low
			  is_rising = false;
		  };

		  systime_t now = chVTGetSystemTimeX();

		  switch(state) {
		  case kWait:
			  if (is_rising == true) {
				  ///maybe start kLeadingPulse
				  state = kLeadingPulse;
				  byte = 0;
				  memset(address_command, 0, sizeof(address_command));
				  bits = 0;
				  address_command_index = 0;
				  start = now;
			  } else {
				  state = kWait;
			  }
			  break;
		  case kLeadingPulse:
			  if (is_rising == false) {
				  /// maybe end kLeadingPulse
				  if (((start + US2ST(8500)) < now) &&
					  (now < (start + US2ST(13000))) ) {
					  /// end kLeadingPulse
					  /// start kLeadingPulseSpace
					  state = kLeadingPulseSpace;
					  start = now;
				  } else {
					  state = kWait;
				  }
			  } else {
				  state = kWait;
			  }
			  break;
		  case kLeadingPulseSpace:
			  if (is_rising == true) {
				  /// maybe end kLeadingPulseSpace
				  if (((start + US2ST(4000)) < now) &&
					  (now < (start + US2ST(5000))) ) {
					  /// end kLeadingPulseSpace
					  /// start kBurst
					  state = kBurst;
					  start = now;
				  } else {
					  state = kWait;
				  }
			  } else {
				  state = kWait;
			  }
			  break;
		  case kBurst:
			  if (is_rising == false) {
				  /// maybe end kBurst
				  if (((start + US2ST(300)) < now) &&
					  (now < (start + US2ST(800))) ) {
					  /// end kBurst
					  /// start kBurstSpace
					  state = kBurstSpace;
					  start = now;
				  } else {
					  state = kWait;
				  }
			  } else {
				  state = kWait;
			  }
			  break;
		  case kBurstSpace:
			  if (is_rising == true) {
				  /// maybe end kBurstSpace
				  if (((start + US2ST(300)) < now) &&
					  (now < (start + US2ST(800))) ) {
					  /// end kBurstSpace
					  /// Logical '0'
					  /// start kBurst
					  set_bit(0);
					  state = kBurst;
					  start = now;
				  } else {
					  if (((start + US2ST(1200)) < now) &&
						  (now < (start + US2ST(2000))) ) {
						  /// end kBurstSpace
						  /// Logical '1'
						  /// start kBurst
						  set_bit(1);
						  state = kBurst;
						  start = now;
					  } else {
						  state = kWait;
					  }
				  }
			  } else {
				  state = kWait;
			  }
			  break;
		  }
	  }

//	  chSysUnlockFromISR();
}

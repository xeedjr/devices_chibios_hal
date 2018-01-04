/*
 * RfReceiverPT2262.cpp
 *
 *  Created on: 2 ñ³÷. 2018 ð.
 *      Author: Bogdan
 */

#include <RfReceiverPT2262.h>

RfReceiverPT2262::RfReceiverPT2262(ioportid_t port, uint8_t pad) {
	// TODO Auto-generated constructor stub
	_port = port;
	_pad = pad;
}

RfReceiverPT2262::~RfReceiverPT2262() {
	// TODO Auto-generated destructor stub
}

void RfReceiverPT2262::set_bit(uint8_t bit) {
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


			static bool tt = true;
			if (tt) {
//					bl->change_fan_speedI(100);
				tt = false;
			} else {
//					bl->change_fan_speedI(0);
				tt = true;
			};

		};
		address_command_index = 0;
	}
}

void RfReceiverPT2262::extcb_gpioa_0(EXTDriver *extp, expchannel_t channel) {
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
				  state = kHi;
				  address_data = 0;
				  bits_index = 0;
				  frame_index = 0;
				  memset(frame, 0, sizeof(frame));
				  start = now;
			  } else {
				  state = kWait;
			  }
			  break;
		  case kHi:
			  if (is_rising == false) {
				  /// maybe end kHi
				  if (((start + US2ST(PERIOD*3)) < now) &&
					  (now < (start + US2ST(PERIOD*5))) ) {
					  /// end kLeadingPulse
					  /// Logical '0'
					  address_data <<= 1;
					  bits_index++;
					  state = kLow;
					  start = now;
				  } else {
					  if (((start + US2ST(PERIOD*10)) < now) &&
						  (now < (start + US2ST(PERIOD*14))) ) {
						  /// end kBurstSpace
						  /// Logical '1'
						  /// start kBurst
						  address_data <<= 1;
						  address_data |= 1;
						  bits_index++;
						  state = kLow;
						  start = now;
					  } else {
						  state = kWait;
					  }
				  }
			  } else {
				  state = kWait;
			  }
			  break;
		  case kLow:
			  if (is_rising == true) {
				  /// maybe end kLeadingPulseSpace
				  if (((start + US2ST(PERIOD*3)) < now) &&
					  (now < (start + US2ST(PERIOD*14))) ) {
					  /// end kLeadingPulseSpace
					  /// start kBurst
					  if (bits_index == 12) {
						  frame[frame_index++] = address_data;
						  address_data = 0;
						  if (frame_index == 4) {
							  state = kWait;
						  }
						  state = kWait;
					  }
					  state = kHi;
					  start = now;
				  } else {
					  state = kWait;
				  }
			  } else {
				  state = kWait;
			  }
			  break;
		  }
	  }

//	  chSysUnlockFromISR();
}

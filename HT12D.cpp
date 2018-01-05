/*
 * HT12D.cpp
 *
 *  Created on: 3 ñ³÷. 2018 ð.
 *      Author: Bogdan
 */

#include "HT12D.h"

HT12D::HT12D(ioportid_t port, uint8_t pad, callback_t callback, GPTDriver* gptd) {
	// TODO Auto-generated constructor stub
	_port = port;
	_pad = pad;
	_callback = callback;
	_gptd = gptd;
}

HT12D::~HT12D() {
	// TODO Auto-generated destructor stub
}

void HT12D::extcb_gpioa_0(EXTDriver *extp, expchannel_t channel) {
	  (void)extp;
	  (void)channel;
}

/// Called every 10us
void HT12D::gpt_cb(GPTDriver *gptp) {
	  (void)gptp;

	  chSysLockFromISR();

//	  palWritePad(_port, 4, 1);
//	  palWritePad(_port, 4, 0);
/*	  static uint8_t tt = 0;
	  if (tt) {
		  palWritePad(_port, 4, 1);
		  tt = 0;
	  } else {
		  palWritePad(_port, 4, 0);
		  tt = 1;
	  };
*/

	  if (timer != 0xFFFF) {
		  timer++;
	  }

	  switch (state) {
	  case kWait:
		  /// count time
		  if (palReadPad(_port, _pad) == 1) {
			  /// Hi
			  if (timer > 300) {
				  state = kSyncPulse;
				  palWritePad(_port, 4, 1);
				  palWritePad(_port, 4, 0);
			  };
			  timer = 0;
		  }
		  break;
	  case kSyncPulse:
		  /// count time
		  if (palReadPad(_port, _pad) == 0) {
			  /// Low
			  if (timer > 12) {
				  state = kData;
				  data = 0;
				  data_index = 0;
				  catch_time = 80;
				  palWritePad(_port, 4, 1);
				  palWritePad(_port, 4, 0);
				  palWritePad(_port, 4, 1);
				  palWritePad(_port, 4, 0);
			  } else {
				  state = kWait;
			  }
			  timer = 0;
		  };
		  break;
	  case  kData:
		  ///catch data
		  if (timer == catch_time) {
			  palWritePad(_port, 4, 1);
			  palWritePad(_port, 4, 0);
			  palWritePad(_port, 4, 1);
			  palWritePad(_port, 4, 0);
			  palWritePad(_port, 4, 1);
			  palWritePad(_port, 4, 0);
			  palWritePad(_port, 4, 1);
			  palWritePad(_port, 4, 0);

			  if (palReadPad(_port, _pad) == 1) {
				  /// Hi
				  data <<= 1;
			  } else {
				  /// Low
				  data <<= 1;
				  data |= 1;
			  };
			  data_index++;
			  if (data_index == 12) {
				  palWritePad(_port, 4, 1);
				  palWritePad(_port, 4, 0);
				  palWritePad(_port, 4, 1);
				  palWritePad(_port, 4, 0);
				  palWritePad(_port, 4, 1);
				  palWritePad(_port, 4, 0);
				  palWritePad(_port, 4, 1);
				  palWritePad(_port, 4, 0);
				  palWritePad(_port, 4, 1);
				  palWritePad(_port, 4, 0);
				  buffer[0] = buffer[1];
				  buffer[1] = buffer[2];
				  buffer[2] = data;
				  if (buffer[2] == buffer[1]) {
					  palWritePad(_port, 4, 1);
					  palWritePad(_port, 4, 0);
					  palWritePad(_port, 4, 1);
					  palWritePad(_port, 4, 0);
					  palWritePad(_port, 4, 1);
					  palWritePad(_port, 4, 0);
					  palWritePad(_port, 4, 1);
					  palWritePad(_port, 4, 0);
					  palWritePad(_port, 4, 1);
					  palWritePad(_port, 4, 0);
					  palWritePad(_port, 4, 1);
					  palWritePad(_port, 4, 0);
					  _callback(buffer[0]);
					  memset(buffer, 0xFFFF, sizeof(buffer));
				  }
				  timer = 0;
				  state = kWait;
			  }
			  if (data_index == 1) {
				  catch_time = 120;
			  }
			  timer = 0;
		  };
		  break;
	  };
//	  gptStartOneShotI(&GPTD3, 1000);   /* 0.1 second pulse.*/
	  chSysUnlockFromISR();
}

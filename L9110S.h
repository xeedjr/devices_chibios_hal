#pragma once

#include "hal.h"

class L9110S {
	ioline_t ia, ib;
public:
	enum DIRECTION {
		kA,
		kB,
		kStop
	};
	void init(ioline_t ia, ioline_t ib);
	void set_direction(DIRECTION dir);
};
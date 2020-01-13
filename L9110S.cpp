#include "L9110S.h"

void L9110S::init(ioline_t ia, ioline_t ib) {
	this->ia = ia;
	this->ib = ib;
	palSetLineMode(ia, PAL_MODE_OUTPUT_PUSHPULL);
	palSetLineMode(ib, PAL_MODE_OUTPUT_PUSHPULL);
}

void L9110S::set_direction(DIRECTION dir) {
	switch (dir) {
	case DIRECTION::kA:
		{
			palSetLine(ia);
			palClearLine(ib);
		}
		break;
	case DIRECTION::kB:
		{
			palSetLine(ib);
			palClearLine(ia);
		}
		break;
	case DIRECTION::kStop:
		{
			palClearLine(ia);
			palClearLine(ib);
		}
		break;
	}
}
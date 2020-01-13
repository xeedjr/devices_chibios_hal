#include "hal.h"

class L9110S {
	enum DIRECTION {
		kA,
		kB,
		kStop
	};
	ioline_t ia, ib;
public:
	void init(ioline_t ia, ioline_t ib);
	void set_direction(DIRECTION dir);
};
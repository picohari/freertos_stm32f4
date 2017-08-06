#ifndef _CALIBRATION_H
#define _CALIBRATION_H

#include <string.h>
#include "gfx.h"


#ifndef UGFXSIMULATOR

// Calibration data
float calibrationData[] = {
	0.06483,		// ax
	-0.00271,		// bx
	-11.5467,		// cy
	0.00313,		// ay
	0.08925,		// by
	-27.9073 		// cy
};

// The loading routine
bool_t LoadMouseCalibration(unsigned instance, void *data, size_t sz)
{
	(void)instance;
	
	if (sz != sizeof(calibrationData)) {
		return FALSE;
	}
	
	memcpy(data, (void*)&calibrationData, sz);
	
	return TRUE;
}

#endif /* UGFXSIMULATOR */


#endif /* _CALIBRATION_H */


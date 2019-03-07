#ifndef CAL_SERVO_H
#define CAL_SERVO_H

#define OFFSET_P 4
#define OFFSET_A 10
#define MIN_ANGLE 60
#define MAX_ANGLE 120
#define REZ 0.04375

#include "general_types.h"

void vSetAngle(T_U8 u8Angle);

#endif
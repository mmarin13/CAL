#ifndef _RTE_H_
#define _RTE_H_
	
#include "hal_lf.h"
#include "cal_servo.h"
#include "hal_motor.h"
#include "mcal_interrupts.h"

#define RTE_vSetAngle vSetAngle
#define RTE_vMotorInit vMotorInit
#define RTE_vSetMotorDir vSetMotorDir
#define RTE_vSetMotorSpeed vSetMotorSpeed
#define RTE_LF_vSetPinsDir LF_vSetPinsDir
#define RTE_LF_vWritePins LF_vWritePins
#define RTE_LF_u8ReadPins LF_u8ReadPins

#endif
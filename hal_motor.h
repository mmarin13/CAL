#ifndef HAL_MOTOR_H
#define HAL_MOTOR_H

#include "general_types.h"
#include "mcal_gpio.h"
#include "mcal_pwm.h"

#define FORWARD 0
#define BACKWARD 1

void vMotorInit(void);
void vSetMotorDir(BOOL u8Dir);
void vSetMotorSpeed(T_F16 u8Speed);

#endif


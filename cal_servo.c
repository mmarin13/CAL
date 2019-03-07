#include "cal_servo.h"
#include "mcal_gpio.h"

void vSetAngle(T_U8 u8Angle)
{
	if(u8Angle < MIN_ANGLE)
	{
		u8Angle = MIN_ANGLE;
	}
	if(u8Angle > MAX_ANGLE)
	{
		u8Angle = MAX_ANGLE;
	}
	T_F16 f16Duty = OFFSET_P + REZ*(u8Angle - OFFSET_A);
	PWM1_vSetDuty(f16Duty, 1);
}

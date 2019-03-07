#include "hal_motor.h"

void vMotorInit(void)
{
	GPIO_u8SetPortPin(PORT_A, 9,DIGITAL, OUTPUT);
}

void vSetMotorDir(BOOL u8Dir)
{
	GPIO_u8WritePortPin(PORT_A, 9, u8Dir);
}

void vSetMotorSpeed(T_F16 f16Speed)
{
	if(f16Speed > 100)
	{
		f16Speed = 100;
	}
	PWM1_vSetDuty(f16Speed, 2);
}
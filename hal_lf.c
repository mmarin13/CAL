#include "hal_lf.h"

void LF_vSetPinsDir(BOOL bDir)
{
	T_U8 nrPin = 0;
	for(nrPin = 0; nrPin <= 5; ++nrPin)
	{
		GPIO_u8SetPortPin(PORT_C,nrPin,DIGITAL,bDir);
	}
}

void LF_vWritePins(void)
{
	T_U8 nrPin = 0;
	for(nrPin = 0; nrPin <= 5; ++nrPin)
	{
		 GPIO_u8WritePortPin(PORT_C, nrPin, 1);
	}
}

T_U8 LF_u8ReadPins(void)
{
	T_U16 temp = 0;
	T_U8 val = 0; 
	T_U16 MASK = 0b111111;
	LF_vSetPinsDir(OUTPUT);
	LF_vWritePins();
	__delay_us(10);
	LF_vSetPinsDir(INPUT);
	__delay_us(250);
	temp = GPIO_u16ReadPort(PORT_C);
	val = temp & MASK;
	return val;
}

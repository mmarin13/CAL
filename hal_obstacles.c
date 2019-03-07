#include "mcal_interrupts.h"

void vSetMotorSpeed(T_F16 u8Speed);

void vHandleObst()
{
    if(bObstDetected)
    {
        vSetMotorSpeed(0);
    }
//    else
//    {
//        vSetMotorSpeed(30);
//    }
}

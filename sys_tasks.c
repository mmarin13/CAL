/* 
 * File:   sys_tasks.h
 * Author: Cristian T. A.
 *
 * Created on August 17, 2018, 1:26 PM
 */

#include "general.h"
#include "sys_tasks.h"
#include "cal_servo.h"
#include "hal_motor.h"
#include "mcal_init.h"
#include "hal_lf.h"
#include "ASW.h"
#include "hal_battery.h"
#include "hal_obstacles.h"

T_F16 battProcentage;

void TASK_Inits()
{
    MCAL_vInit();
    GPIO_u8SetPortPin(PORT_A, 10, DIGITAL, OUTPUT);	
	vInit_Everything();
}

void TASK_1ms()
{
    
}

void TASK_5ms()
{
	
}

void TASK_10ms()
{   
	
}

void TASK_100ms()
{ 
    vFollower();
}
 
void TASK_500ms()
{ 
    battProcentage = u8ReadBatteryProcentage();
}

void TASK_1000ms()
{

}
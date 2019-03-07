/* 
 * File:   hal_battery.c
 * Author: student
 *
 * Created on February 21, 2019, 6:34 PM
 */

#include "hal_battery.h"

T_F16 u8ReadBatteryProcentage()
{
    T_F16 a[2][4] = { {8.4, 7, 6.5, 6},
                      {100, 20, 10, 0} };
    T_F16 refVoltage = 3.07;
    T_U16 maxValue = 4095;    
    
    T_U16 readValue =  ADC_u16Read(0);
    
    T_F16 readVoltage = (float)readValue/maxValue * refVoltage;
    readVoltage = readVoltage * 4; //scale
    
    T_U8 index = 0;
    for(index = 1; index < 4; index++) //find the correct interval
    {
        if(readVoltage > a[0][index])
            break;
    }
    if(index == 4) //if battery voltage is less than 6V
        return 0;
    
    T_F16 rez = (a[1][index-1] - a[1][index])/(a[0][index-1] - a[0][index]); //resolution
    
    T_F16 batteryProcentage = 100 - (float)rez*(a[0][index-1] - readVoltage);
    return batteryProcentage;
}

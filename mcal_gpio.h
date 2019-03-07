/* 
 * File:   mcal_gpio.h
 * Author: Nicolae Hagiu
 *
 * Created on August 21, 2028, 8:57 AM
 */

#ifndef MCAL_GPIO_H
#define	MCAL_GPIO_H

#include "general.h"


#define ANALOG  0
#define DIGITAL 1

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define PORT_A 'A'
#define PORT_B 'B'
#define PORT_C 'C'


void GPIO_u8SetPortPin(T_U8 u8Port,T_U8 u8Pin,BOOL bAD,BOOL bDir);
void GPIO_u8WritePortPin(T_U8 u8Port,T_U8 u8Pin,BOOL bValue);
T_U8 GPIO_u8ReadPortPin(T_U8 u8Port,T_U8 u8Pin);
void GPIO_vWritePort(T_U8 u8Port,T_U16 s16Value);
T_U16 GPIO_u16ReadPort(T_U8 u8Port);



#endif	/* MCAL_GPIO_H */


/* 
 * File:   general.h
 * Author: Cristian T. A.
 *
 * Created on August 1, 2018, 11:14 AM
 */


#ifndef GENERAL_H
#define	GENERAL_H

//In MPLab8 change to 
#include <p33Fxxxx.h>
#include "general_types.h"
#include "mcal_gpio.h"

/* Instruction Cycle Clock = 3.7 MHz */
#define FCY 3700000
#include <libpic30.h>

#define LEDr_ON GPIO_u8WritePortPin(PORT_A, 10, 1);
#define LEDr_OFF GPIO_u8WritePortPin(PORT_A, 10, 0);
#define LEDr_TOG GPIO_u8WritePortPin(PORT_A, 10, ~GPIO_u8ReadPortPin(PORT_A, 10));

#define LEDn_ON GPIO_u8WritePortPin(PORT_A, 8, 0);
#define LEDn_OFF GPIO_u8WritePortPin(PORT_A, 8, 1);    
#define LEDn_TOG GPIO_u8WritePortPin(PORT_A, 8, ~GPIO_u8ReadPortPin(PORT_A, 8));

#define LEDa_ON GPIO_u8WritePortPin(PORT_B, 8, 0);
#define LEDa_OFF GPIO_u8WritePortPin(PORT_B, 8, 1);    
#define LEDa_TOG GPIO_u8WritePortPin(PORT_B, 8, ~GPIO_u8ReadPortPin(PORT_B, 8));

#define LEDp_ON GPIO_u8WritePortPin(PORT_C, 7, 0);
#define LEDp_OFF GPIO_u8WritePortPin(PORT_C, 7, 1);    
#define LEDp_TOG GPIO_u8WritePortPin(PORT_C, 7, ~GPIO_u8ReadPortPin(PORT_C, 7));


#endif	/* GENERAL_H */


/* 
 * File:   mcal_init.h
 * Author: uia94881
 *
 * Created on August 10, 2018, 3:22 PM
 */

#ifndef MCAL_INIT_H
#define	MCAL_INIT_H

#include "mcal_osc.h"
#include "mcal_adc.h"
#include "mcal_pwm.h"
#include "mcal_interrupts.h"
#include "mcal_spi.h"
#include "mcal_timers.h"
#include "mcal_encoder.h"


/* Microcontroller modules initialization function */
void MCAL_vInit(void);

#endif	/* MCAL_INIT_H */


/* 
 * File:   mcal_pwm.h
 * Author: Nicolae Hagiu (uia95516)
 *
 * Created on August 8, 2028, 10:37 AM
 */

#ifndef MCAL_PWM_H
#define	MCAL_PWM_H

#include "general.h"

void PWM1_vInit(void);
void PWM2_vInit(void);
void PWM1_vSetPeriod(T_U16 u16Period_ms );
void PWM2_vSetPeriod(T_U16 u16Period_ms );
void PWM1_vSetDuty(T_F16 f16Duty,T_U8 u8Ch);
void PWM2_vSetDuty(T_F16 f16Duty);
void PWM1_vSetDutyPeriod(T_U16 u16Period_ms,T_F16 f16Duty);
void PWM2_vSetDutyPeriod(T_U16 u16Period_ms,T_F16 f16Duty);

/* P1TCONbits.PTCKPS = 3 PWM  timer pre-scale 1 set 64 */
#define valPrescal_1 64 

/* P1TCONbits.PTCKPS = 3, PWM timer pre-scale 2 set 64 */
#define valPrescal_2 64  

#endif	/* MCAL_PWM_H */


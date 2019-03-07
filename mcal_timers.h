/* 
 * File:   mcal_timers.h
 * Author: Cristian T. A.
 *
 * Created on August 16, 2018, 12:46 PM
 */

#ifndef MCAL_TIMERS_H
#define	MCAL_TIMERS_H


#define TIM2_PERIOD_MS 0.00216216
#define TIM3_PERIOD_MS 0.0692

#define TIMER_PRESCALER_1   0
#define TIMER_PRESCALER_8   1
#define TIMER_PRESCALER_64  2
#define TIMER_PRESCALER_256 3

/************************************************/
/* DO NOT USE THESE TIMERS FOR YOUR APP. *******/
/* THEY ARE USED TO IMPLEMENT RECURRENT TASKs */
/******** USE GENERAL PURPOSE TIMERS INSTEAD */
/********************************************/
/* TIMER MODULE _2_ */
void TIM2_vInit(void);
void TIM2_vStart(void);
void TIM2_vStop(void);
void TIM2_vResetCount(void);
void TIM2_vSetCount(T_U16 u16Count);
void TIM2_vSetCountLimit(T_U16 u16CountLimit);
void TIM2_vSetCountLimit_1ms();
void TIM2_vSetTime(T_U8 u8Miliseconds);
T_U16 TIM2_u16GetCount();

/* TIMER MODULE _3_ */
void TIM3_vInit(void);
void TIM3_vStart(void);
void TIM3_vStop(void);
void TIM3_vResetCount(void);
void TIM3_vSetCount(T_U16 u16Count);
void TIM3_vSetCountLimit(T_U16 u16CountLimit);
void TIM3_vSetTime(T_U16 u16Miliseconds);
T_U16 TIM3_u16GetCount(void);

/*******************************************/
/***** GENERAL PURPOSE  TIMER MODULES *****/
/*****************************************/
/****************************************/
/* HOW TO USE  TIMERS ***************************/
/***********************************************/
/* 0. Init() with the prescaler value you want. 
 *  This will affect the frequency of the timer.*/
/* 1. Set the CountLimit to the desired value. 
 *  This is the maximum value the timer will reach. 
 * After reaching this value, the timer interrupt occurs.*/
/* 2. Set your flags in the interrupt service routine (ISR), 
 *  and process them in your application. 
 *  ! Don't forget to reset your flags after processing. ! */
/* 3. Start() the timer. **********************************/
/* 4. The interrupt will occur when 
 *  the timer will reach your CountLimit value. *************/
/* 5. Use the timer someway, somehow, somewhere. Good Luck!*/
/**********************************************************/
/* TIMER MODULE _4_ */
void TIM4_vInit(T_U8 u8Prescaler);
void TIM4_vStart(void);
void TIM4_vStop(void);
void TIM4_vResetCount(void);
void TIM4_vSetCount(T_U16 u16Count);
void TIM4_vSetCountLimit(T_U16 u16CountLimit);
T_U16 TIM4_u16GetCount(void);

/* TIMER MODULE _5_ */
void TIM5_vInit(T_U8 u8Prescaler);
void TIM5_vStart(void);
void TIM5_vStop(void);
void TIM5_vResetCount(void);
void TIM5_vSetCount(T_U16 u16Count);
void TIM5_vSetCountLimit(T_U16 u16CountLimit);
T_U16 TIM5_u16GetCount(void);

#endif	/* MCAL_TIMERS_H */


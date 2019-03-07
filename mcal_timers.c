/* 
 * File:   mcal_timers.c
 * Author: Cristian T. A.
 *
 * Created on August 16, 2018, 12:46 PM
 */

#include "general.h"
#include "mcal_timers.h"


T_U16 u16Tim2IntCnt = 0;
T_U16 u16Tim3IntCnt = 0;
BOOL u8Tim2IntFlag = FALSE;
BOOL u8Tim3IntFlag = FALSE;

/*******************************************************************************/
/*******************************           *************************************/
/******************************    TIMER 2    **********************************/
/*******************************           *************************************/
/*******************************************************************************/
/***********************************************************************************************************************
** Timer 2  ISR ** 
******************
** 
** Resets(clears) the interrupt flag of Timer 2;
**  and increments interrupt count. 
***********************************************************************************************************************/
void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void)
{
    ++u16Tim2IntCnt;
    /* must be cleared by user app. */
    u8Tim2IntFlag = TRUE;
    IFS0bits.T2IF = FALSE;  /* Clear Timer 2 Interrupt Flag */
}
/***** Usage: *****/
/******************/
/* In main(): 
 * while(1U) 
 * { 
 *  if(1 == u8Tim2IntFlag)
 *  {
 *      ....etc
 *      ...
 *      u8Tim2IntFlag = 0;
 *  }
 *  
 * }*/
/********************************************/
/********************************************/

/***********************************************************************************************************************
*  Function name    : TIM2_vInit
*
*  Description      : Initializes Timer 2 module. 
*
*              Timer counts at a frequency = Fcy/8.
*          TIM2_vStart() starts the counting. 
* 
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void TIM2_vInit()
{     
    TMR2 = 0;
    
    T2CONbits.TON = 0;
    T2CONbits.TSIDL = 0;
    T2CONbits.TGATE = 0;
    T2CONbits.TCKPS = TIMER_PRESCALER_8;
    T2CONbits.T32 = 0;
    T2CONbits.TCS = 0;
    /********************************************/
    /* LSb to MSb                               */
    /* TCS   = 0    : Internal Clock (Fcy)      */
    /* T32   = 0    : 16 bit timer              */
    /* TCKPS = 1    : 1:8 prescaler             */
    /* TGATE = 0    : Gate disabled             */
    /* TSIDL = 0    : Continue in Idle Mode     */
    /* TON   = 0    : Stop Timer                */
    /*                                          */
    /********************************************/
    
    
    IPC1bits.T2IP = 0x01;
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
    /* call TIM2_vStart() to start Timer */
}

/***********************************************************************************************************************
*  Function name    : TIM2_vStart
*
*  Description      : Starts the timer module.
*
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void TIM2_vStart()
{
    /* bit TON = 1 */
    T2CON = T2CON | 0x8000;  
}

/***********************************************************************************************************************
*  Function name    : TIM2_vStop
*
*  Description      : Stops the timer module (from counting).
*
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void TIM2_vStop()
{
    /* bit TON = 0 */
    T2CON = T2CON & 0x7FFF; 
}

/***********************************************************************************************************************
*  Function name    : TIM2_vResetCount
*
*  Description      : Resets the counting. (counter = 0)
*
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void TIM2_vResetCount()
{
    TMR2 = 0;
}

/***********************************************************************************************************************
*  Function name    : TIM2_vSetCount
*
*  Description      : Sets the value of the counter. 
*
*  List of arguments: T_U16 u16Count. - the value to assign to the counter. 
*
*  Return value     : -
*
***********************************************************************************************************************/
void TIM2_vSetCount(T_U16 u16Count)
{
    TMR2 = u16Count;
}

/***********************************************************************************************************************
*  Function name    : TIM2_vSetCountLimit
*
*  Description      : Sets the top value for the counter. 
* 
*                    Ttimer = 0.00(216) ms * 
*    u16CountLimit * Ttimer = Time elapsed until Timer interrupt. 
*
*  List of arguments: T_U16 u16CountLimit.
* 
*          The timer will count up until this number; and generate interrupt.
* 
*  Return value     : -
*
***********************************************************************************************************************/
void TIM2_vSetCountLimit(T_U16 u16CountLimit)
{
    PR2 = u16CountLimit;
}

/***********************************************************************************************************************
*  Function name    : TIM2_vSetCountLimit_1ms
*
*  Description      : Sets the top value for the counter, 
*               so that when the timer starts counting from 0, 
*           it needs 1 ms to reach top value; and generate interrupt
* 
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void TIM2_vSetCountLimit_1ms()
{
    PR2 = 462;
    
    /* 462 = 463 - 1 */
    /*********************************/
    /* Why '463', right ?!? Take a look: 
     * 
     *   Ftimer = 3.7 MHz / 8 (prescaler) = 462.5 KHz. 
     *   Ttimer = 1/Ftimer = 0.00(216) ms 
     *   463 * Ttimer = 1.00(108) ms. */
    /*********************************/
    /* Why '-1' ?!? :
     *  Timer reaches value in PR2 and it sets the Flag.
     *  After another 1 clock cycle, it start counting from 0. */
    /**********************************************************/
    /*    In other words: 
     * It takes 463 clock cycles to count 1 ms, only the first time after starting Timer. 
     * After, it takes 462 clock cycles. */
    /*********************************************************/
    /* =>  See Note from Example 11-1, Section 11 - Timers; dsPIC33 Datasheet. */
}

/***********************************************************************************************************************
*  Function name    : TIM2_vSetTime
*
*  Description      : Sets the number of miliseconds after which 
*              the timer interrupts.
* 
*  List of arguments: T_U8 u8Miliseconds    
* 
*           1 <= u8Miliseconds <= 141
*
*  Return value     : -
*
***********************************************************************************************************************/
void TIM2_vSetTime(T_U8 u8Miliseconds)
{
    if(1 <= u8Miliseconds && 141 >= u8Miliseconds)
    {
        PR2 = (T_U16)((T_F16)u8Miliseconds / TIM2_PERIOD_MS);
    }
}

/***********************************************************************************************************************
*  Function name    : TIM2_u16GetCount
*
*  Description      : Gets the value of the counter.
*
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
T_U16 TIM2_u16GetCount()
{
    return TMR2;
}


/*******************************************************************************/
/*******************************           *************************************/
/******************************    TIMER 3    **********************************/
/*******************************           *************************************/
/*******************************************************************************/

/***********************************************************************************************************************
** Timer 3  ISR ** 
******************
** 
** Resets(clears) the interrupt flag of Timer 3;
**  and increments interrupt count.  
***********************************************************************************************************************/
void __attribute__((__interrupt__, no_auto_psv)) _T3Interrupt(void)
{
    ++u16Tim3IntCnt;
    u8Tim3IntFlag = TRUE;
    IFS0bits.T3IF = FALSE;  /* Clear Timer 3 Interrupt Flag */
}
/***** Usage: *****/
/******************/
/* In main(): 
 * while(1U) 
 * { 
 *  if(1 == u8Tim3IntFlag)
 *  {
 *      ....etc
 *      ...
 *      u8Tim3IntFlag = 0;
 *  }
 *  
 * }*/
/********************************************/
/********************************************/

/***********************************************************************************************************************
*  Function name    : TIM3_vInit
*
*  Description      : Initializes Timer 3 module. 
*
*              Timer counts at a frequency = Fcy/256.
*          TIM3_vStart() starts the counting. 
* 
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void TIM3_vInit()
{     
    TMR3 = 0;
    
    T3CONbits.TON = 0;
    T3CONbits.TSIDL = 0;
    T3CONbits.TGATE = 0;
    T3CONbits.TCKPS = TIMER_PRESCALER_256;
    T3CONbits.TCS = 0;
    /********************************************/
    /* LSb to MSb                               */
    /* TCS   = 0    : Internal Clock (Fcy)      */
    /* T32   = 0    : 16 bit timer              */
    /* TCKPS = 3    : 1:256 prescaler           */
    /* TGATE = 0    : Gate disabled             */
    /* TSIDL = 0    : Continue in Idle Mode     */
    /* TON   = 0    : Stop Timer                */
    /*                                          */
    /********************************************/
    
    
    IPC2bits.T3IP = 0x02;
    IFS0bits.T3IF = 0;
    IEC0bits.T3IE = 1;
    /* call TIM3_vStart() to start Timer */
}

/***********************************************************************************************************************
*  Function name    : TIM3_vStart
*
*  Description      : Starts the timer module.
*
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void TIM3_vStart()
{
    /* bit TON = 1 */
    T3CON = T3CON | 0x8000;
}

/***********************************************************************************************************************
*  Function name    : TIM3_vStop
*
*  Description      : Stops the timer module (from counting).
*
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void TIM3_vStop()
{
    /* bit TON = 0 */
    T3CON = T3CON & 0x7FFF; 
}

/***********************************************************************************************************************
*  Function name    : TIM3_vResetCount
*
*  Description      : Resets the counting. (counter = 0)
*
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void TIM3_vResetCount()
{
    TMR3 = 0;
}

/***********************************************************************************************************************
*  Function name    : TIM3_vSetCount
*
*  Description      : Sets the value of the counter. 
*
*  List of arguments: T_U16 u16Count. - the value to assign to the counter. 
*
*  Return value     : -
*
***********************************************************************************************************************/
void TIM3_vSetCount(T_U16 u16Count)
{
    TMR3 = u16Count;
}

/***********************************************************************************************************************
*  Function name    : TIM3_vSetCountLimit
*
*  Description      : Sets the top value for the counter. 
* 
*                    Ttimer = 0.0692 ms 
*    u16CountLimit * Ttimer = Time elapsed until Timer interrupt. 
*
*  List of arguments: T_U16 u16CountLimit.
* 
*          The timer will count up until this number; and generate interrupt.
* 
*  Return value     : -
*
***********************************************************************************************************************/
void TIM3_vSetCountLimit(T_U16 u16CountLimit)
{
    PR3 = u16CountLimit;
}

/***********************************************************************************************************************
*  Function name    : TIM3_vSetTime
*
*  Description      : Sets the number of miliseconds after which 
*              the timer interrupts.
* 
*  List of arguments: T_U16 u16Miliseconds    
* 
*           1 <= u8Miliseconds <= 4535
*
*  Return value     : -
*
***********************************************************************************************************************/
void TIM3_vSetTime(T_U16 u16Miliseconds)
{
    if(1 <= u16Miliseconds && 4535 >= u16Miliseconds)
    {
        PR3 = (T_U16)(u16Miliseconds / TIM3_PERIOD_MS);
    }
}


/***********************************************************************************************************************
*  Function name    : TIM3_u16GetCount
*
*  Description      : Gets the value of the counter.
*
*  List of arguments: -
*
*  Return value     : T_U16, counter value
*
***********************************************************************************************************************/
T_U16 TIM3_u16GetCount()
{
    return TMR3;
}


/*******************************************************************************/
/*******************************           *************************************/
/******************************    TIMER 4    **********************************/
/*******************************           *************************************/
/*******************************************************************************/

/***********************************************************************************************************************
** Timer 4  ISR ** 
******************
** 
** Resets(clears) the interrupt flag of Timer 4;
***********************************************************************************************************************/
void __attribute__((__interrupt__, no_auto_psv)) _T4Interrupt(void)
{
    IFS1bits.T4IF = FALSE;  /* Clear Timer 4 Interrupt Flag */
      
    /* insert code here */
}

/***********************************************************************************************************************
*  Function name    : TIM4_vInit
*
*  Description      : Initializes Timer 4 module. 
*
*              Timer counts at a frequency = Fcy/u8Prescaler.
*          TIM4_vStart() starts the counting. 
* 
*  List of arguments: T_U8 u8Prescaler. 
* 
*          Should be one of the ALREADY DEFINED VALUES: TIMER_PRESCALER_x.
*
*  Return value     : -
*
***********************************************************************************************************************/
void TIM4_vInit(T_U8 u8Prescaler)
{     
    TMR4 = 0;
    
    T4CONbits.TON = 0;
    T4CONbits.TSIDL = 0;
    T4CONbits.TGATE = 0;
    T4CONbits.TCKPS = u8Prescaler;
    T4CONbits.TCS = 0;
    /********************************************/
    /* LSb to MSb                               */
    /* TCS   = 0    : Internal Clock (Fcy)      */
    /* T32   = 0    : 16 bit timer              */
    /* TCKPS = u8Prescaler: 1:prescaler         */
    /* TGATE = 0    : Gate disabled             */
    /* TSIDL = 0    : Continue in Idle Mode     */
    /* TON   = 0    : Stop Timer                */
    /*                                          */
    /********************************************/
    
    
    IPC6bits.T4IP = 0x04;
    IFS1bits.T4IF = 0;
    IEC1bits.T4IE = 1;
    
    /* call TIM4_vStart() to start Timer */
}

/***********************************************************************************************************************
*  Function name    : TIM4_vStart
*
*  Description      : Starts the timer module.
*
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void TIM4_vStart()
{
    /* bit TON = 1 */
    T4CON = T4CON | 0x8000;
}

/***********************************************************************************************************************
*  Function name    : TIM4_vStop
*
*  Description      : Stops the timer module (from counting).
*
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void TIM4_vStop()
{
    /* bit TON = 0 */
    T4CON = T4CON & 0x7FFF; 
}

/***********************************************************************************************************************
*  Function name    : TIM4_vResetCount
*
*  Description      : Resets the counting. (counter = 0)
*
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void TIM4_vResetCount()
{
    TMR4 = 0;
}

/***********************************************************************************************************************
*  Function name    : TIM4_vSetCount
*
*  Description      : Sets the value of the counter. 
*
*  List of arguments: T_U16 u16Count. - the value to assign to the counter. 
*
*  Return value     : -
*
***********************************************************************************************************************/
void TIM4_vSetCount(T_U16 u16Count)
{
    TMR4 = u16Count;
}

/***********************************************************************************************************************
*  Function name    : TIM4_vSetCountLimit
*
*  Description      : Sets the top value for the counter. 
* 
*                    Ttimer = 0.0692 ms 
*    u16CountLimit * Ttimer = Time elapsed until Timer interrupt. 
*
*  List of arguments: T_U16 u16CountLimit.
* 
*          The timer will count up until this number; and generate interrupt.
* 
*  Return value     : -
*
***********************************************************************************************************************/
void TIM4_vSetCountLimit(T_U16 u16CountLimit)
{
    PR4 = u16CountLimit;
}

/***********************************************************************************************************************
*  Function name    : TIM4_u16GetCount
*
*  Description      : Gets the value of the counter.
*
*  List of arguments: -
*
*  Return value     : T_U16, counter value
*
***********************************************************************************************************************/
T_U16 TIM4_u16GetCount()
{
    return TMR4;
}

/*******************************************************************************/
/*******************************           *************************************/
/******************************    TIMER 5    **********************************/
/*******************************           *************************************/
/*******************************************************************************/

/***********************************************************************************************************************
** Timer 5  ISR ** 
******************
** 
** Resets(clears) the interrupt flag of Timer 5;
***********************************************************************************************************************/
void __attribute__((__interrupt__, no_auto_psv)) _T5Interrupt(void)
{
    IFS1bits.T5IF = FALSE;  /* Clear Timer 5 Interrupt Flag */
      
    /* insert code here */
}

/***********************************************************************************************************************
*  Function name    : TIM5_vInit
*
*  Description      : Initializes Timer 4 module. 
*
*              Timer counts at a frequency = Fcy/u8Prescaler.
*          TIM4_vStart() starts the counting. 
* 
*  List of arguments: T_U8 u8Prescaler. 
* 
*          Should be one of the ALREADY DEFINED VALUES: TIMER_PRESCALER_x.
*
*  Return value     : -
*
***********************************************************************************************************************/
void TIM5_vInit(T_U8 u8Prescaler)
{     
    TMR5 = 0;
    
    T5CONbits.TON = 0;
    T5CONbits.TSIDL = 0;
    T5CONbits.TGATE = 0;
    T5CONbits.TCKPS = u8Prescaler;
    T5CONbits.TCS = 0;
    /********************************************/
    /* LSb to MSb                               */
    /* TCS   = 0    : Internal Clock (Fcy)      */
    /* T32   = 0    : 16 bit timer              */
    /* TCKPS = u8Prescaler: 1:prescaler         */
    /* TGATE = 0    : Gate disabled             */
    /* TSIDL = 0    : Continue in Idle Mode     */
    /* TON   = 0    : Stop Timer                */
    /*                                          */
    /********************************************/
    
    
    IPC7bits.T5IP = 0x05;
    IFS1bits.T5IF = 0;
    IEC1bits.T5IE = 1;
    
    /* call TIM5_vStart() to start Timer */
}

/***********************************************************************************************************************
*  Function name    : TIM5_vStart
*
*  Description      : Starts the timer module.
*
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void TIM5_vStart()
{
    /* bit TON = 1 */
    T5CON = T5CON | 0x8000;
}

/***********************************************************************************************************************
*  Function name    : TIM5_vStop
*
*  Description      : Stops the timer module (from counting).
*
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void TIM5_vStop()
{
    /* bit TON = 0 */
    T5CON = T5CON & 0x7FFF; 
}

/***********************************************************************************************************************
*  Function name    : TIM4_vResetCount
*
*  Description      : Resets the counting. (counter = 0)
*
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void TIM5_vResetCount()
{
    TMR5 = 0;
}

/***********************************************************************************************************************
*  Function name    : TIM5_vSetCount
*
*  Description      : Sets the value of the counter. 
*
*  List of arguments: T_U16 u16Count. - the value to assign to the counter. 
*
*  Return value     : -
*
***********************************************************************************************************************/
void TIM5_vSetCount(T_U16 u16Count)
{
    TMR5 = u16Count;
}

/***********************************************************************************************************************
*  Function name    : TIM5_vSetCountLimit
*
*  Description      : Sets the top value for the counter. 
* 
*                    Ttimer = 0.0692 ms 
*    u16CountLimit * Ttimer = Time elapsed until Timer interrupt. 
*
*  List of arguments: T_U16 u16CountLimit.
* 
*          The timer will count up until this number; and generate interrupt.
* 
*  Return value     : -
*
***********************************************************************************************************************/
void TIM5_vSetCountLimit(T_U16 u16CountLimit)
{
    PR5 = u16CountLimit;
}

/***********************************************************************************************************************
*  Function name    : TIM5_u16GetCount
*
*  Description      : Gets the value of the counter.
*
*  List of arguments: -
*
*  Return value     : T_U16, counter value
*
***********************************************************************************************************************/
T_U16 TIM5_u16GetCount()
{
    return TMR5;
}
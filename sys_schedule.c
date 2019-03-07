/* 
 * File:   sys_schedule.c
 * Author: Cristian T. A.
 *
 * Created on August 20, 2018, 9:45 AM
 */

#include "general.h"
#include "sys_tasks.h"
#include "mcal_timers.h"
#include "sys_schedule.h"


BOOL u8FlagTask_5ms;
BOOL u8FlagTask_10ms;
BOOL u8FlagTask_500ms;
BOOL u8FlagTask_1000ms;

extern T_U16 u16Tim2IntCnt;
extern T_U16 u16Tim3IntCnt;
extern BOOL u8Tim2IntFlag;
extern BOOL u8Tim3IntFlag;


/***********************************************************************************************************************
*  Function name    : TASK_vSchedule
* 
*  Description      : Sets and starts the timers. 
*                        In while(1U) loop, the function polls the flags indicating if any interrupts occurred.
*                     
*              When the right time has come, the function calls the corresponding task. 
* 
*  List of arguments: -
* 
*  Return value     : -
***********************************************************************************************************************/
void TASK_vSchedule()
{
    /* Interrupt every 1ms */
    TIM2_vSetCountLimit_1ms();
    /* Interrupt every 100ms */
    TIM3_vSetTime(100);
    
    u8FlagTask_5ms = FALSE;
    u8FlagTask_10ms = FALSE;
    u8FlagTask_500ms = FALSE;
    u8FlagTask_1000ms = FALSE;
    
    u8Tim2IntFlag = FALSE;
    u8Tim3IntFlag = FALSE;
    
    u16Tim2IntCnt = 0;
    u16Tim3IntCnt = 0;
    
    
    /* Start timers. */
    TIM2_vStart();
    TIM3_vStart();
    
    while(1U)
    {
        /**********************/
        /* 1, 5, 10 ms check */
        if(TRUE == u8Tim2IntFlag)
        {
            u8Tim2IntFlag = FALSE;
            
            if(5 == u16Tim2IntCnt)
            {
                u8FlagTask_5ms = TRUE;
            }
            else if(10 <= u16Tim2IntCnt)
            {
                u16Tim2IntCnt = 0;
                
                u8FlagTask_5ms = TRUE;
                u8FlagTask_10ms = TRUE;
            }
            
            TASK_1ms();
        }
        
        
        /**************/
        /* 5 ms task */
        if(TRUE == u8FlagTask_5ms)
        {
            u8FlagTask_5ms = FALSE;
            
            TASK_5ms();
        }
        
        /***************/
        /* 10 ms task */
        if(TRUE == u8FlagTask_10ms)
        {
            u8FlagTask_10ms = FALSE;
            
            TASK_10ms();
        }

        
        
        /****************************/
        /* 100, 500, 1000 ms check */
        if(TRUE == u8Tim3IntFlag)
        {
            u8Tim3IntFlag = FALSE;
            
            if(5 == u16Tim3IntCnt)
            {
                u8FlagTask_500ms = TRUE;
            }
            else if(10 <= u16Tim3IntCnt)
            {
                u16Tim3IntCnt = 0;
                
                u8FlagTask_500ms = TRUE;
                u8FlagTask_1000ms = TRUE;
            }
            
            TASK_100ms();
        }
        
        /****************/
        /* 500 ms task */
        if(TRUE == u8FlagTask_500ms)
        {
            u8FlagTask_500ms = FALSE;
            
            TASK_500ms();
        }
        
        /*****************/
        /* 1000 ms task */
        if(TRUE == u8FlagTask_1000ms)
        {
            u8FlagTask_1000ms = FALSE;
            
            TASK_1000ms();
        }
    }
}





/****************************************/
/* TEST FUNCTIONS **********************/
/**************************************/
void test_pin_setup()
{
    /* Pin set as digital pin. */
    AD1PCFGL |= (1 << 4);
    
    TRISBbits.TRISB2 = 0;
    TRISAbits.TRISA7 = 0;
    TRISCbits.TRISC6 = 0;
}

void test_vToggle_CSN()
{
    PORTBbits.RB2 = ~LATBbits.LATB2;
}

void test_vToggle_CE()
{
    PORTAbits.RA7 = ~LATAbits.LATA7;
}

void test_vToggle_IRQ()
{
    PORTCbits.RC6 = ~LATCbits.LATC6;
}
/***************************************/
/***************************************/

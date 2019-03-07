#include "mcal_pwm.h"


/*******************************************************************************
*  Function name    : PWM1_vInit
*  Description      : 1)Initializes the PWM1 Module . Channels 1 and 2  are
*                       used ,3 is not used but is initialized .                   
*  List of arguments: no arguments
*  Return value     : no return value
********************************************************************************/

void PWM1_vInit(void)
{
    /* make sure PWM pins are set to be outputs*/
    TRISB =TRISB &0x83FF;
    /* clear the outputs*/ 
    PORTB =PORTB & 0x83FF;                            
    
    /* PWM1 timer post-scale */
    P1TCONbits.PTOPS  = 1;
    /* PWM1 timer pre-scale (value_prescaler =64) */
    P1TCONbits.PTCKPS = 3; 
    /* PWM1 time base operates in a Free-Running mode */
    P1TCONbits.PTMOD  = 0;                           
    
    /* PWM1 counter value, start at 0 */
    P1TMR = 0;                                        
    
    /* PWM1 in complimentary mode ,channel 3 */
    PWM1CON1bits.PMOD3  = 0;
    /* PWM1 in complimentary mode ,channel 2 */
    PWM1CON1bits.PMOD2  = 0;
    /* PWM1 in Independent mode ,channel 1 */
    PWM1CON1bits.PMOD1  = 1; 
    /* PWM1 High pin is enabled (RB10) */
    PWM1CON1bits.PEN3H  = 1;
    /* PWM1 High pin is enabled (RB12) */
    PWM1CON1bits.PEN2H  = 1;
    /* PWM1 High pin is enabled (RB14) */
    PWM1CON1bits.PEN1H  = 1;
    /* PWM1 Channel 1, Low pin is disabled (RB15) */
    PWM1CON1bits.PEN1L  = 0;
    /* Reassign channel A to RP15 */
    RPINR14bits.QEA1R = 15;
    /* PWM1 Low pin is enabled (RB11) */
    PWM1CON1bits.PEN3L  = 1; 
     
    
    /* Enable PWM1 */
    P1TCONbits.PTEN = 1;                             
    
    /* set default period=20 milliseconds  */
    PWM1_vSetPeriod(20);                              
}
/*******************************************************************************
*  Function name    : PWM2_vInit
*  Description      : 1)Initializes the PWM2 Module . Channels 1 is
*                       used  .                   
*  List of arguments: no arguments
*  Return value     : no return value
********************************************************************************/
void PWM2_vInit(void)
{
    /* make sure PWM2 pins are set to be outputs */
    TRISCbits.TRISC6 =0;  
    TRISCbits.TRISC7 =0;
    /* clear the outputs*/
    PORTCbits.RC6    =0;                            
    PORTCbits.RC7    =0;                             
    
    /* PWM2 timer post-scale*/
    P2TCONbits.PTOPS  = 1;
    /* PWM2 timer pre-scale value_prescaler=64 */
    P2TCONbits.PTCKPS = 3;
    /* PWM2 time base operates in a Free-Running mode */
    P2TCONbits.PTMOD  = 0;                            
    
    /* PWM2 counter value, start at 0 */
    P2TMR = 0;                                        
    
    /* PWM2 in complimentary mod*/
    PWM2CON1bits.PMOD1  = 0;
     /* PWM Low pin enabled (RC7) */
    PWM2CON1bits.PEN1L  = 1;                            
    
    /* Enable PWM2 Time base! */
    P2TCONbits.PTEN = 1;                              
    
     
}
/*******************************************************************************
*  Function name    : PWM1_vSetPeriod
*  Description      : Set period PWM1 with a period which is in milliseconds 
*                                          
*  List of arguments: u16TimePeriod_ms expressed in milliseconds 
*  Return value     : no return value
********************************************************************************/
void PWM1_vSetPeriod(T_U16 u16Period_ms )
{
  T_U16 aux;                                          
  
  /*calculation value for load in P1TPER register  */
  aux=u16Period_ms*(FCY/valPrescal_1/1000)-1;
  
  /*PTPER can be loaded with a maximum value of 32768 */
  if(aux<32768)                                    
       P1TPER=aux; /* PWM1 Time base period*/   
}
/*******************************************************************************
*  Function name    : PWM2_vSetPeriod
*  Description      : Set period PWM2 with a period which is in milliseconds 
*                                          
*  List of arguments: u16TimePeriod_ms expressed in milliseconds 
*  Return value     : no return value
********************************************************************************/
void PWM2_vSetPeriod( T_U16 u16Period_ms )
{
  T_U16 aux;                                          
  
  /* calculation value for load in P1TPER register  */
  aux=u16Period_ms*(FCY/valPrescal_2/1000)-1;
  
  /* PTPER can be loaded with a maximum value of 32768 */
  if(aux<32768)                                    
       P2TPER=aux; /* PWM1 Time base period */                                   
}
/*******************************************************************************
*  Function name    : PWM1_vSetDuty
*  Description      : Set  Duty cycle PWM1 with a value which express 
*                      a percentage of the period                     
*  List of arguments: f16duty ,u8Ch 
*  Return value     : no return value
********************************************************************************/
void PWM1_vSetDuty(T_F16 f16Duty, T_U8 u8Ch)
{
    T_U16 u16Period_ms;
    
    /* calculation period */
    u16Period_ms=(P1TPER+1)/(FCY/valPrescal_1/1000);
    
    if(f16Duty==0) 
     {
        switch(u8Ch)
        {
            case 1:P1DC1=0;
                   break;
            case 2:P1DC2=0;
                   break;
            case 3:P1DC3=0;
                   break;
            default:
                    break;
        }
     }
    else
    if(f16Duty<=100)
    {
     switch(u8Ch)
        {
        case 1:P1DC1=(u16Period_ms*(T_F16)(FCY/valPrescal_1/1000))/100*2*f16Duty-1;
           break;
        case 2:P1DC2=(u16Period_ms*(T_F16)(FCY/valPrescal_1/1000))/100*2*f16Duty-1;
           break;
        case 3:P1DC3=(u16Period_ms*(T_F16)(FCY/valPrescal_1/1000))/100*2*f16Duty-1;
           break;
        default:
            break;
        }
    }    
}
/*******************************************************************************
*  Function name    : PWM2_vSetDuty
*  Description      : Set Duty cycle PWM2 with a value which express 
*                    a percentage of the period                     
*  List of arguments: f16duty 
*  Return value     : no return value
********************************************************************************/
void PWM2_vSetDuty(T_F16 f16Duty)
{
    T_U16 u16Period_ms;
    
    /*calculation period*/
    u16Period_ms=(P1TPER+1)/(FCY/valPrescal_1/1000);
    
    if(f16Duty==0)
        P2DC1=0;
    else
        if(f16Duty<=100)
           P2DC1=(u16Period_ms*(T_F16)(FCY/valPrescal_2/1000))/100*2*f16Duty-1;
}

/******************************************************************************
*  Function name    : PWM1_vSetDutyPeriod
*  Description      : Set Duty cycle PWM1 for all channels with a value which 
*                     express a percentage of the period 
*                     Set period PWM1 for all channels
*  List of arguments: u16Period_ms , f16duty 
*  Return value     : no return value
********************************************************************************/
void PWM1_vSetDutyPeriod(T_U16 u16Period_ms,T_F16 f16Duty)
{
  /* set period for all channels*/
  PWM1_vSetPeriod(u16Period_ms);
  /* set duty  for chanel 1 */
  PWM1_vSetDuty(f16Duty,1);
  /* set duty for chanel 2 */
  PWM1_vSetDuty(f16Duty,2);
  /* set duty for chanel 3 */
  PWM1_vSetDuty(f16Duty,3);
}
/*******************************************************************************
*  Function name    : PWM2_vSetDutyPeriod.
*  Description      : Set Duty cycle PWM2  with a value which 
*                    express a percentage of the period. 
*                    Set period PWM2 .
*  List of arguments: u16Period_ms , f16duty .
*  Return value     : no return value.
********************************************************************************/
void PWM2_vSetDutyPeriod(T_U16 u16Period_ms,T_F16 f16Duty)
{
  PWM2_vSetPeriod(u16Period_ms);
  PWM2_vSetDuty(f16Duty); 
}


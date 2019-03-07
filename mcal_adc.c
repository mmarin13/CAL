
#include "mcal_adc.h"


/*******************************************************************************
*  Function name    : ADC_Init
*  Description      : 1)Initializes the ADC Module in 12bit mode. Channel CH0 is
*                       is used because only it can perform 12bit conversions.
*                     2)Sets AN0(RA0),AN1(RA1) as Inputs, Analog Mode. 
*                     3)Sets the sample time.
*                     4)Enables the ADC Module
*                   
*  List of arguments: no arguments
*  Return value     : no return value
********************************************************************************/
void ADC_vInit()
{
    /* AN0(RA0),AN1(RA1)-> Input Pins */
    TRISAbits.TRISA0=1;     
    TRISAbits.TRISA1=1;
    
    /* AN0, AN1 in Analog-Mode */
    AD1PCFGLbits.PCFG0 = 0;  
    AD1PCFGLbits.PCFG1 = 0; 
    
    /* Make sure the Registers values are 0 */
    AD1CON1=0x0000;
    AD1CON2=0x0000; 
    
    /* 12bit conversion */
    AD1CON1bits.AD12B=1;    
    
    /* Auto Sample Time bits->31Tad */ 
    AD1CON3=0x1F00;
    
    /* Enable ADC Module */  
    AD1CON1bits.ADON=1;          
}


/*******************************************************************************
*  Function name    : ADC_u16Read
*  Description      : Sets the ANx pin as CH0 positive input.
*                     Samples and Converts the ANx pin.
*                                     
*  List of arguments: T_U8 ANx 
*  Return value     : Unsigned Short -> Buffer Value(0...4095)
********************************************************************************/
T_U16 ADC_u16Read(T_U8 ANx)
{
    if(AD1CHS0bits.CH0SA!=ANx)
    {
        /* CH0 positive input is ANx */
        /* Use only AN0(RA0) or AN1(RA1) */
        /* AN2->AN8 coresponding pins have other functions */
        AD1CHS0bits.CH0SA=ANx;  
    }
    /* Start Sampling */
    AD1CON1bits.SAMP=1;
    __delay_us(50);
    
    /* Stop Sampling. Start Conversion */
    AD1CON1bits.SAMP=0;
    
    /* Conversion Finished? */
    while(!AD1CON1bits.DONE);
    
    /* Read Buffer */
    return ADC1BUF0;            
}

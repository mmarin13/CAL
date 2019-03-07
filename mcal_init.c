#include "mcal_init.h"

/*******************************************************************************
*  Function name    : init_vModules
*  Description      : Initializes all Modules         
*  List of arguments: no arguments  
*  Return value     : no return value
********************************************************************************/
void MCAL_vInit()
{
    OSC_vInit();
    ADC_vInit();
    PWM1_vInit();
    INT_vInit(); 
    SPI_RF_vInit();
    QEI_vInit();
    TIM2_vInit();
    TIM3_vInit();
    
}
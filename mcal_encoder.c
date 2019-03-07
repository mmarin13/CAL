#include "mcal_encoder.h"


/*******************************************************************************
*  Function name    : Encoder_vInit
*  Description      : 1)Initializes the QEI(Quadrature Encoder Interface)
*                     2)Sets the count register value in the middle for easier 
*                       data processing(by decreasing count reset probability)          
*  List of arguments: no arguments
*  Return value     : no return value
********************************************************************************/
void QEI_vInit(void)
{
    /* Set channel A and B microcontroller pins as Inputs */
    TRISCbits.TRISC9 = 1;
    TRISBbits.TRISB15 = 1;
    
    /* Assign channel A to RP15 */
    RPINR14bits.QEA1R = 15;
    /* Assign channel B to RP25 */
    RPINR14bits.QEB1R = 25;
    
    /* Timer Input Clock Prescale 1:64 */
    QEI1CONbits.TQCKPS = 0b10;           
    /* Encoder enabled (x4 mode) with position counter reset by match(MAXxCNT) */
    QEICONbits.QEIM = 0b111; 
    /* Set count register value around the middle of the possible value range */
    POS1CNT=32000;
    /* Count value can be read from POS1CNT register */
}

/*******************************************************************************
*  Function name    : Encoder_u16getCount
*  Description      : Returns POS1CNT(count register) value        
*  List of arguments: no arguments
*  Return value     : T_U16 -> value of POS1CNT.
*                     POS1CNT is initialized at 32000
*                     POS1CNT is: incremented when moving backwards
*                                 decremented when moving forward 
********************************************************************************/
T_U16 QEI_u16getCount()
{
    return POS1CNT;
}


/*******************************************************************************
*  Function name    : QEI_vResetCount
*  Description      : Resets the count register to 32000         
*  List of arguments: no arguments
*  Return value     : no return value  
********************************************************************************/
void QEI_vResetCount()
{
    POS1CNT=32000;
}

T_S16 QEI_s16GetElapsed()
{
    static T_U16 prevCount = 3200;
    T_U16 count = QEI_u16getCount();
    T_S16 elapsed = prevCount - count;  //get difference of impulses
    QEI_vResetCount(); //reset counter    
    prevCount = count;  //update previous counter
    elapsed = elapsed / 100; //convert into cm
    return elapsed;
}


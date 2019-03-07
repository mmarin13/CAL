/*
 * File:   mcal_osc.c
 * Author: Cristian T. A.
 *
 * Created on August 1, 2018, 10:07 AM
 */


#include "general.h"


/* Set Clock Source = Internal FRC */
#pragma config FNOSC = FRC


void OSC_vInit()
{
    /* Tune Clock Frequency from 7.37 MHz to 7.4 MHz */
    OSCTUNbits.TUN = 1;   
}


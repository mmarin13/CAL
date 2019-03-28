/* 
 * File:   asw_com.c
 * Author: Cristian T. A.
 *
 * Created on August 28, 2018, 9:37 AM
 */

#include "general.h"
#include "RTE.h"
#include "asw_com.h"

extern struct _car car;

/* nRF variables */
extern T_U8 au8RxBuff[]; 
extern T_U8 u8RxBuff_Index;
extern BOOL u8NewRX;

/***********************************************************************************************************************
*  Function name    : COM_vCheckIRQ
*
*  Description      : Checks if there is a new received message OR if the transmission is complete.
*                            
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void COM_vCheckIRQ()
{
    RTE_RF_vCheckIRQ();
}

/***********************************************************************************************************************
*  Function name    : COM_vSendMessage
*
*  Description      : Puts the RF module in the Transmit State, to transmit the message.
*                            
*  List of arguments: T_U8 u8Message - the message to be transmitted.
*
*  Return value     : -
*
***********************************************************************************************************************/
void COM_vSendMessage(T_U8 u8Message)
{
  RTE_RF_vBeginTransmit(u8Message);
}

/***********************************************************************************************************************
*  Function name    : COM_vStartListening
*
*  Description      : Puts the RF module in the Receive State.
*                            
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void COM_vStartListening()
{
    RTE_RF_vBeginReceive();
}

/****************************************/
/* Message decoding ********************/
/**************************************/

/***********************************************************************************************************************
*  Function name    : COM_vProcessMessage
*
*  Description      : Takes action depending on the message type and the current state 
*          of the Application's Finite State Machine (APP_FSM).
*                            
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void COM_vProcessMessage(T_U8 u8Message)
{
    T_U8 u8OtherIdRoad = u8Message && 0x03;
    T_U8 u8OtherDirection = (u8Message && 0x0C) >> 2;
    T_U8 u8OtherDone = (u8Message & 0x10) >> 4;
    if(u8OtherDone) 
    {
        car.bWait = FALSE;
    }
    else
    {
        switch(car.u8IdRoad)
        {
            case 0:
                switch(car.direction)
                {
                    case LEFT:
                        if((u8OtherIdRoad == 2 && (u8OtherDirection == CENTER || u8OtherDirection == RIGHT)) || (u8OtherIdRoad == 3))
                            car.bWait = TRUE;
                        break;
                    case CENTER:
                        if(u8OtherIdRoad == 3)
                            car.bWait = TRUE;
                        break;
                    case RIGHT:
                        car.bWait = FALSE;
                }
                break;
            case 1:
                switch(car.direction)
                {
                    case LEFT:
                        if((u8OtherIdRoad == 3 && (u8OtherDirection == CENTER || u8OtherDirection == RIGHT)) || (u8OtherIdRoad == 0))
                            car.bWait = TRUE;
                        break;
                    case CENTER:
                        if(u8OtherIdRoad == 0)
                            car.bWait = TRUE;
                        break;
                    case RIGHT:
                        car.bWait = FALSE;
                }
                break;
            case 2:
                switch(car.direction)
                {
                    case LEFT:
                        if((u8OtherIdRoad == 0 && (u8OtherDirection == CENTER || u8OtherDirection == RIGHT)) || (u8OtherIdRoad == 1))
                            car.bWait = TRUE;
                        break;
                    case CENTER:
                        if(u8OtherIdRoad == 1)
                            car.bWait = TRUE;
                        break;
                    case RIGHT:
                        car.bWait = FALSE;
                }
                break;
            case 3:
                switch(car.direction)
                {
                    case LEFT:
                        if((u8OtherIdRoad == 1 && (u8OtherDirection == CENTER || u8OtherDirection == RIGHT)) || (u8OtherIdRoad == 2))
                            car.bWait = TRUE;
                        break;
                    case CENTER:
                        if(u8OtherIdRoad == 2)
                            car.bWait = TRUE;
                        break;
                    case RIGHT:
                        car.bWait = FALSE;
                }
                break;
        }
    }
    
}

/***********************************************************************************************************************
*  Function name    : COM_vProcessFIFO
*
*  Description      : Processes every message in the message queue, if there is any available.
*                            
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void COM_vProcessFIFO()
{
    if(TRUE == u8NewRX)
    {
        u8NewRX = FALSE;
        
        T_U8 i;
        for(i = 0; i < u8RxBuff_Index; ++i)
        {
            COM_vProcessMessage(au8RxBuff[i]);
        } 
        u8RxBuff_Index = 0;
    }
}


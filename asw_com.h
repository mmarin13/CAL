/* 
 * File:   asw_com.h
 * Author: Cristian T. A.
 *
 * Created on August 28, 2018, 9:38 AM
 */

#ifndef ASW_COM_H
#define	ASW_COM_H

#define ASW_COM_DIM_FIFO 64


void COM_vCheckIRQ(void);
void COM_vSendMessage(T_U8 u8Message);
void COM_vStartListening(void);
void COM_vProcessMessage(T_U8 u8Message);
void COM_vProcessFIFO(void);


#endif	/* ASW_COM_H */


/* 
 * File:   mcal_spi.h
 * Author: Cristian T. A.
 *
 * Created on August 14, 2018, 10:27 AM
 */

#ifndef MCAL_SPI_H
#define	MCAL_SPI_H

#define RF_CMD_mask_WriteReg 0x20
#define RF_CMD_mask_ReadReg 0x00
#define RF_CMD_mask_WriteToTX 0xA0
#define RF_CMD_mask_ReadFromRX 0x61
#define RF_CMD_mask_ReadPayloadCount 0x60
#define RF_CMD_mask_TX_Flush 0xE1
#define RF_CMD_mask_RX_Flush 0xE2
#define RF_CMD_mask_NOP 0xFF
#define RF_CMD_mask_ACTIVATE 0x50

#define RF_CMD_ReadStatus 0
#define RF_CMD_ReadReg8bit 1
/* #define RF_CMD_ReadReg40bit 2 */
#define RF_CMD_WriteReg8bit 3
/* #define RF_CMD_WriteReg40bit 4 */
/* #define RF_CMD_RX_GetPayload 5 */
#define RF_CMD_TX_SetPayload 6
#define RF_CMD_TX_Flush 7
#define RF_CMD_RX_Flush 8
/* #define RF_CMD_ReuseTxPayload 9 */
#define RF_CMD_Activate 10

/* SPI function declarations */
void SPI_RF_vInit(void);
BOOL SPI_RF_u8RxBuffOvf(void);
BOOL SPI_RF_u8TxBuffFull(void);
BOOL SPI_RF_u8RxBuffFull(void);
void SPI_RF_vDisable(void);
T_U8 SPI_RF_u8TxByte(T_U8);
T_U8 SPI_RF_u8TxByteByte(T_U8, T_U8);
T_U8 SPI_RF_u8TxCmd(T_U8, T_U8, T_U8);
T_U8 SPI_RF_u8ReadReg_5Bytes(T_U8[], T_U8);
T_U8 SPI_RF_u8WriteReg_5Bytes(T_U8[], T_U8);


#endif	/* MCAL_SPI_H */


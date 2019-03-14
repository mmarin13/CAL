/* 
 * File:   hal_nRF.h
 * Author: Cristian T. A.
 *
 * Created on August 21, 2018, 10:54 AM
 */

#ifndef HAL_NRF_H
#define	HAL_NRF_H


/*******************************/
/* nRF regs. internal address */
/*****************************/
#define RF_REGADDR__CONFIG 0x00
#define RF_REGADDR__EN_AA 0x01
#define RF_REGADDR__EN_RXADDR 0x02
#define RF_REGADDR__SETUP_AW 0x03
#define RF_REGADDR__SETUP_RETR 0x04
#define RF_REGADDR__RF_CH 0x05
#define RF_REGADDR__RF_SETUP 0x06
#define RF_REGADDR__STATUS 0x07
#define RF_REGADDR__OBSERVE_TX 0x08
#define RF_REGADDR__CD 0x09
#define RF_REGADDR__RX_ADDR_P0 0x0A
#define RF_REGADDR__RX_ADDR_P1 0x0B
#define RF_REGADDR__RX_ADDR_P2 0x0C
#define RF_REGADDR__RX_ADDR_P3 0x0D
#define RF_REGADDR__RX_ADDR_P4 0x0E
#define RF_REGADDR__RX_ADDR_P5 0x0F
#define RF_REGADDR__TX_ADDR 0x10
#define RF_REGADDR__RX_PW_P0 0x11
#define RF_REGADDR__RX_PW_P1 0x12
#define RF_REGADDR__RX_PW_P2 0x13
#define RF_REGADDR__RX_PW_P3 0x14
#define RF_REGADDR__RX_PW_P4 0x15
#define RF_REGADDR__RX_PW_P5 0x16
#define RF_REGADDR__FIFO_STATUS 0x17
#define RF_REGADDR__FEATURE 0x1D
/* REG ADDR > 0x17 RESERVED - DO NOT MODIFY.*/

/*******************************/
/* Bit positions in nRF regs. */
/*****************************/
#define RF_REG_RFSETUP_bit_LNA_HCURR 0
#define RF_REG_RFSETUP_bit_RF_PWR 1
#define RF_REG_RFSETUP_bit_RF_DR 3
#define RF_REG_RFSETUP_bit_PLL_LOCK 4
/* RF_SETUP bits 5:7 RESERVED */

#define RF_REG_FIFOSTATUS_bit_RX_EMPTY 0x01
#define RF_REG_FIFOSTATUS_bit_RX_FULL 0x02
#define RF_REG_FIFOSTATUS_bit_TX_EMPTY 0x10
#define RF_REG_FIFOSTATUS_bit_TX_FULL 0x20

#define RF_REG_STATUS_bit_RX_DR 0x40
#define RF_REG_STATUS_bit_TX_DS 0x20
#define RF_REG_STATUS_bit_MAX_RT 0x10

/*******************************/
/* Config. Values for our APP */
/*****************************/
/* RF_NO_AUTO_ACK is used for the case of 1 TX, multiple RX. */
#define RF_NO_AUTO_ACK 0

#define RF_APP_CFG__RX_DATA_PIPES 1
/* here, RF_APP_CFG__AUTO_ACK_PIPES can be either: RF_NO_AUTO_ACK  or  RF_APP_CFG__RX_DATA_PIPES. */
#define RF_APP_CFG__AUTO_ACK_PIPES RF_NO_AUTO_ACK
#define RF_APP_CFG__ADDR_WIDTH 5
#define RF_APP_CFG__AUTO_ReTx_DELAY 0
#define RF_APP_CFG__AUTO_ReTx_COUNT 3

#define RF_APP_CFG__OPERATING_RFCHANNEL 1

#define RF_APP_CFG__AIR_DATARATE RF_AIR_DATA_RATE_2Mbps
#define RF_APP_CFG__OUTPUT_POWER RF_OUT_POW_0dB
#define RF_APP_CFG__LNA_GAIN 1

#define RF_APP_CFG__PAYLOAD_LENGTH 1

#define RF_OUT_POW_minus18dB 0
#define RF_OUT_POW_minus12dB 1
#define RF_OUT_POW_minus6dB 2
#define RF_OUT_POW_0dB 3

#define RF_AIR_DATA_RATE_1Mbps 0
#define RF_AIR_DATA_RATE_2Mbps 1
/*******************************/
/* Min. and max. values of HW */
/*****************************/
#define RF_MIN_PIPES 0
#define RF_MAX_PIPES 5
#define RF_MIN_ADDR_WIDTH 3
#define RF_MAX_ADDR_WIDTH 5
#define RF_MIN_RETX 0
#define RF_MAX_RETX 15
#define RF_MAX_PAYLD_LEN 32

#define APP_RXBUFF_DIM 32
/*************************/

typedef enum {
    modePwrDown = 0,
    modeStandBy = 1,
    modeTX = 2,
    modeRX = 3
}enRF_OPMODE;

/* * * Functions called in RF_vInit() * * */
void RF_vCloseAllPipes(void);
void RF_vOpenPipeZero(void);

void RF_vSet_RxPipes(T_U8 u8NrOfPipes);
void RF_vSet_AutoAck(T_U8 u8NrOfPipes);
void RF_vSet_AddressWidth(T_U8 u8NrOfBytes);
void RF_vSet_AutoRetransmit(T_U8 u8Delay, T_U8 u8Count);
void RF_vSet_RF_Channel(T_U8 u8Channel);
void RF_vSet_RF_Setup(T_U8 u8AirDataRate, T_U8 u8OutputPower, T_U8 u8LnaGain);
void RF_vEnableActivateReg(void);
void RF_vDisableFeatureReg(void);
void RF_vSet_PayloadLen(T_U8 u8Length);
void RF_vSet_TxAddr(T_U8 au8Address[]);
void RF_vSet_PrimaryRxAddr(T_U8 au8Address[]);
void RF_vSet_RxAddr(T_U8 u8PipeNumber, T_U8 au8Address[]);
void RF_vSet_TxRxAddress(T_U8 au8Address[]);

void RF_vFirstPowerUp(void);
/* * * * * * * * */
void RF_vInit(void);

void RF_vSetStandByMode(void);
T_U8 RF_u8Get_Status(void);
T_U8 RF_u8Get_ObsTx(void);
T_U8 RF_u8Get_FifoStatus(void);
void RF_vFlushTX(void);
void RF_vFlushRX(void);

void RF_vLoadTxFIFO(T_U8 u8Data);
void RF_vSetTxMode(void);
void RF_vBeginTransmit(T_U8 u8Data);

void RF_vSetRxMode(void);
void RF_vBeginReceive(void);
void RF_vGetReceivedData(void);

void RF_vCheckIRQ(void);

/*****************/
/* Test methods */
void test_vTransmitter(void);
void test_vReceiver(void);

#endif	/* HAL_NRF_H */


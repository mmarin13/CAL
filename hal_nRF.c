#include "general.h"
#include "mcal_spi.h"
#include "hal_nRF.h"

T_U8 au8RxBuff[APP_RXBUFF_DIM];
/* Position of the next free index. */
T_U8 u8RxBuff_Index;

enRF_OPMODE eRF_OpMode;
BOOL bRF_IRQ;
BOOL u8NewRX;


/***********************************************************************************************************************
*  Function name    : RF_vCloseAllPipes
*
*  Description      : Disables Auto-Acknowledge on all pipes. Closes all pipes.
*                            
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void RF_vCloseAllPipes(void)
{
    /* Disable Auto-Acknowledge for all Pipes. */
    SPI_RF_u8TxCmd(RF_CMD_WriteReg8bit, RF_REGADDR__EN_AA, 0x00);
    /* Close all pipes. */
    SPI_RF_u8TxCmd(RF_CMD_WriteReg8bit, RF_REGADDR__EN_RXADDR, 0x00);    
}

/***********************************************************************************************************************
*  Function name    : RF_vOpenPipeZero
*
*  Description      : Opens Pipe 0. Enables Auto-Acknowledge for RX Pipe 0.
* 
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void RF_vOpenPipeZero(void)
{
    /* Open RX Pipe 0 */
    SPI_RF_u8TxCmd(RF_CMD_WriteReg8bit, RF_REGADDR__EN_RXADDR, 0x01);
    /* Enable Auto-Acknowledge for RX Pipe 0 */
    SPI_RF_u8TxCmd(RF_CMD_WriteReg8bit, RF_REGADDR__EN_AA, 0x01);
}

/***********************************************************************************************************************
*  Function name    : RF_vSet_RxPipes
*
*  Description      : Enables the first  - u8NrOfPipes -  RX pipes
* 
*                      See nRF's Register Map - EN_RXADDR
*
*  List of arguments: T_U8 u8NrOfPipes. 0 < u8NrOfPipes <= 5
*
*  Return value     : -
*
***********************************************************************************************************************/
void RF_vSet_RxPipes(T_U8 u8NrOfPipes)
{
    T_U8 u8RegVal = 0;
    
    if(u8NrOfPipes > RF_MIN_PIPES && u8NrOfPipes <= RF_MAX_PIPES)
    {
        T_U8 i;
        for(i = 0; i  < u8NrOfPipes; ++i)
        {
            u8RegVal = u8RegVal | (1 << i);
        }
    }
    
    /* Open the first - u8NrOfPipes - RX Pipes. */
    SPI_RF_u8TxCmd(RF_CMD_WriteReg8bit, RF_REGADDR__EN_RXADDR, u8RegVal);
}

/***********************************************************************************************************************
*  Function name    : RF_vSet_AutoAck
*
*  Description      : Enables Auto Acknowledge for the first  - u8NrOfPipes -  RX pipes
* 
*                       See nRF's Register Map - EN_AA
* 
*  List of arguments: T_U8 u8NrOfPipes. 0 < u8NrOfPipes <= 5
*
*  Return value     : -
*
***********************************************************************************************************************/
void RF_vSet_AutoAck(T_U8 u8NrOfPipes)
{
    T_U8 u8RegVal = 0;
    
    if(RF_MIN_PIPES < u8NrOfPipes  && RF_MAX_PIPES >= u8NrOfPipes )
    {
        T_U8 i;
    
        for(i = 0; i  < u8NrOfPipes; ++i)
        {
            u8RegVal = u8RegVal | (1 << i);
        }
    }
    
    /* Enable Auto-Acknowledge for the first - u8NrOfPipes - RX Pipes. */
    SPI_RF_u8TxCmd(RF_CMD_WriteReg8bit, RF_REGADDR__EN_AA, u8RegVal);    
}

/***********************************************************************************************************************
*  Function name    : RF_vSet_AddressWidth
*
*  Description      : Sets the Address Length. Can be 3 or 4 or 5 bytes.
* 
*                      See nRF's Register Map - SETUP_AW
*
*  List of arguments: T_U8 u8NrOfBytes. 3 <= u8NrOfBytes <= 5
*
*  Return value     : -
*
***********************************************************************************************************************/
void RF_vSet_AddressWidth(T_U8 u8NrOfBytes)
{
    if(u8NrOfBytes >= RF_MIN_ADDR_WIDTH && u8NrOfBytes <= RF_MAX_ADDR_WIDTH)
    {
        SPI_RF_u8TxCmd(RF_CMD_WriteReg8bit, RF_REGADDR__SETUP_AW, u8NrOfBytes - 2);
    }
}

/***********************************************************************************************************************
*  Function name    : RF_vSet_AutoRetx
*
*  Description      : Sets the Auto Retransmit Enable. 
*                     Sets the delay between retransmits.
*
*                  See nRF's Register Map - SETUP_RETR
* 
*  List of arguments: T_U8 u8Delay, T_U8 u8Count. 
*                  
*  0 <= u8Delay <= 15          0 <= u8Count <= 15
* 
*   Delay                Count
*   0 = 250 us           0 = Retransmit disabled 
*   1 = 500 us           1 = 1 retransmit
*   2 = 750 us           2 = 2 retransmits 
*   3 = 1000 us          3 = 3 retransmits
*   ...                  ...
*   15 = 4000 us         15 = 15 retransmits
*
*  Return value     : -
*
***********************************************************************************************************************/
void RF_vSet_AutoRetx(T_U8 u8Delay, T_U8 u8Count)
{
    if(RF_MAX_RETX >= u8Delay && RF_MIN_RETX <= u8Delay)
    {
        if(RF_MAX_RETX >= u8Count && RF_MIN_RETX <= u8Count)
        {
            SPI_RF_u8TxCmd(RF_CMD_WriteReg8bit, RF_REGADDR__SETUP_RETR, (u8Delay << 4) | u8Count);
        }
    }
}

/***********************************************************************************************************************
*  Function name    : RF_vSet_RF_Channel
*
*  Description      : Sets the RF Channel (frequency band) through which communication will be made
* 
*                  See Register Map - RF_CH 
*
*  List of arguments: T_U8 u8Channel.   0 <= u8Channel < 128
*
*  Return value     : -
*
***********************************************************************************************************************/
void RF_vSet_RF_Channel(T_U8 u8Channel)
{
    if(128 > u8Channel)
    {
        SPI_RF_u8TxCmd(RF_CMD_WriteReg8bit, RF_REGADDR__RF_CH, u8Channel);
    }
}

/***********************************************************************************************************************
*  Function name    : RF_vSet_RF_Setup
*
*  Description      : Sets the Air Data Rate (1 Mbps / 2 Mbps), Output Power and LNAgain.
*
*                   See Register Map - RF_SETUP
* 
*  List of arguments: T_U8 u8AirDataRate, T_U8 u8OutputPower, T_U8 u8LNAgain
*
*  Return value     : -
*
***********************************************************************************************************************/
void RF_vSet_RF_Setup(T_U8 u8AirDataRate, T_U8 u8OutputPower, T_U8 u8LNAgain)
{
    if(0 == u8AirDataRate || 1 == u8AirDataRate)
    {
        if(0 <= u8OutputPower && 3 >= u8OutputPower)
        {
            if(0 == u8LNAgain || 1 == u8LNAgain)
            {
                SPI_RF_u8TxCmd(RF_CMD_WriteReg8bit, RF_REGADDR__RF_SETUP, (u8AirDataRate << RF_REG_RFSETUP_bit_RF_DR) | (u8OutputPower << RF_REG_RFSETUP_bit_RF_PWR) | u8LNAgain);
            }
        }
    }
}

/***********************************************************************************************************************
*  Function name    : RF_vEnableActivateReg
*
*  Description      : Enables Enhanced ShockBurst feature.
*
*                      See Command Set - ACTIVATE
*                       
*                      Call this function again to Deactivate.
* 
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void RF_vEnableActivateReg()
{
    SPI_RF_u8TxCmd(RF_CMD_Activate, 0, 0);
}

/***********************************************************************************************************************
*  Function name    : RF_vDisableFeatureReg
*
*  Description      : Configures the Feature Register. 
* 
*                  See Register Map - FEATURE
*
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void RF_vDisableFeatureReg()
{
    SPI_RF_u8TxCmd(RF_CMD_WriteReg8bit, RF_REGADDR__FEATURE, 0x00);
}

/***********************************************************************************************************************
*  Function name    : RF_vSet_PayloadLen
*
*  Description      : Set the Payload Length 
*               
*                  See Register Map - RX_PW_P0    
*  
*  List of arguments: T_U8 u8Length
* 
*                  Payload Length = u8Length byte(s).   0 = Pipe Disabled. 
*                       max 32 bytes.
* 
*  Return value     : -
*
***********************************************************************************************************************/
void RF_vSet_PayloadLen(T_U8 u8Length)
{
    if(RF_MAX_PAYLD_LEN >= u8Length)
    {
        SPI_RF_u8TxCmd(RF_CMD_WriteReg8bit, RF_REGADDR__RX_PW_P0, u8Length);
    }
}

/***********************************************************************************************************************
*  Function name    : RF_vSet_TxAddr
*
*  Description      : Sets the Transmit Address of the nRF Module.
*
 *                      See Register Map - TX_ADDR 
 * 
*  List of arguments: T_U8 au8Address[]. 5 bytes-long !
*
*  Return value     : -
*
***********************************************************************************************************************/
void RF_vSet_TxAddr(T_U8 au8Address[])
{
    SPI_RF_u8WriteReg_5Bytes(au8Address, RF_REGADDR__TX_ADDR);
}

/***********************************************************************************************************************
*  Function name    : RF_vSet_PrimaryRxAddr
*
*  Description      : Sets the Primary Receive Address (Primary = Pipe 0)
*
*                      RX Pipe 0 = Primary RX
* 
*                      See Register Map - RX_ADDR_P0 
* 
*  List of arguments: T_U8 au8Address[]. 5 bytes-long ! 
*
*  Return value     : -
*
***********************************************************************************************************************/
void RF_vSet_PrimaryRxAddr(T_U8 au8Address[])
{
    SPI_RF_u8WriteReg_5Bytes(au8Address, RF_REGADDR__RX_ADDR_P0);
}

/***********************************************************************************************************************
*  Function name    : RF_vSet_RxAddr
*
*  Description      : Sets the Receive Address for the u8PipeNumber.
*
*  List of arguments: T_U8 u8PipeNumber, T_U8 au8Address[]
*                    0 < u8PipeNumber <= 5 
*  Return value     : -
*
***********************************************************************************************************************/
void RF_vSet_RxAddr(T_U8 u8PipeNumber, T_U8 au8Address[])
{
    /* 0 < u8PipeNumber <= 5 */
    if((RF_MIN_PIPES < u8PipeNumber) && (RF_MAX_PIPES >= u8PipeNumber))
    {
        SPI_RF_u8WriteReg_5Bytes(au8Address, RF_REGADDR__RX_ADDR_P0 + u8PipeNumber);
    }
}

/***********************************************************************************************************************
*  Function name    : RF_vSet_TxRxAddress
*
*  Description      : Sets the same address for RX and TX, on pipe number 0. This enables the Auto-Acknowledge feature.
*
*  List of arguments: T_U8 au8Address[]
*
*  Return value     : -
*
***********************************************************************************************************************/
void RF_vSet_TxRxAddress(T_U8 au8Address[])
{
    /* used for Enhanced Shockburst with Auto-Acknowledge */
    RF_vSet_TxAddr(au8Address);
    RF_vSet_PrimaryRxAddr(au8Address);
}

/***********************************************************************************************************************
*  Function name    : RF_vFirstPowerUp
*
*  Description      : Initializes the RF module for its first power up. (also masks interrupts)
*
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void RF_vFirstPowerUp()
{
    /* IRQ = RX_DR, TX_DS, MAX_RT 
     * EN_CRC = 1, PWR_UP = 1 */
    SPI_RF_u8TxCmd(RF_CMD_WriteReg8bit, RF_REGADDR__CONFIG, 0x0A);
    /* wait 1.5 ms */
    __delay_us(1500);   
    /* entered Standby-I state */
}

/***********************************************************************************************************************
*  Function name    : RF_vInit
*
*  Description      : Initializes the RF module. Sets the primary TX and RX address.
*
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void RF_vInit()
{ 
    /* LED output. nRF testing */
    GPIO_u8SetPortPin(PORT_A, 10, DIGITAL, OUTPUT);
    
    bRF_IRQ = FALSE;
    u8NewRX = FALSE;
    
    T_U8 i;
    for(i=0; i < APP_RXBUFF_DIM; ++i)
    {
        au8RxBuff[i] = 0x00;
    }
    u8RxBuff_Index = 0;
    
    /* TX and RX address */
    T_U8 au8Addr[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
    
    /* Reset Interrupt Request flag */
    bRF_IRQ = FALSE;
    
    /* nRF's state: Power Down. see Datasheet, Radio Control. */
    eRF_OpMode = modePwrDown;
    
    /* Closes all RX Pipes. (0 - 5)*/
    RF_vCloseAllPipes();
    
    /* Enables the RX pipes. */
    RF_vSet_RxPipes(RF_APP_CFG__RX_DATA_PIPES);
    
    /* Enables Auto-Retransmit feature for TX. */
    RF_vSet_AutoRetx(RF_APP_CFG__AUTO_ReTx_DELAY, RF_APP_CFG__AUTO_ReTx_COUNT);
    
    /* Enable Auto-Acknowledge feature when RX. */
    RF_vSet_AutoAck(RF_APP_CFG__AUTO_ACK_PIPES);
    
    /* Sets the address size (3, 4, 5 bytes) */
    RF_vSet_AddressWidth(RF_APP_CFG__ADDR_WIDTH);
    
    /* Sets the radio frequency bandwidth (channel). */
    RF_vSet_RF_Channel(RF_APP_CFG__OPERATING_RFCHANNEL);
    
    /* Sets the payload dimension. (1 - 32 bytes). */
    RF_vSet_PayloadLen(RF_APP_CFG__PAYLOAD_LENGTH);
    
    /* Sets the speed (1, 2 Mbps), transmit power, LNA gain. */
    RF_vSet_RF_Setup(RF_APP_CFG__AIR_DATARATE, RF_APP_CFG__OUTPUT_POWER, RF_APP_CFG__LNA_GAIN);
    
    /* Sets the address for TX and for the RX Pipe 0 (PRX).*/
    /* Same address => Enables Enhanched ShockBurst feature. */
    RF_vSet_TxRxAddress(au8Addr);

    /* Flush the Buffers. */
    RF_vFlushTX();
    RF_vFlushRX();
    
    /* Powers up the module, to StandBy-I Mode. */
    /* Masks the IRQ sources. (TX_DS, RX_DR, MAX_RT) */
    RF_vFirstPowerUp();
    
    /* nRF should be in StandBy-I state. */
    eRF_OpMode = modeStandBy;
}

/***********************************************************************************************************************
*  Function name    : RF_vSetStandByMode
*
*  Description      : Sets the RF module in StandBy-I mode. See RF datasheet. 
*
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void RF_vSetStandByMode()
{
    /* CE to LOW */
    GPIO_u8WritePortPin(PORT_A, 7, LOW);
    
    eRF_OpMode = modeStandBy;
}

/***********************************************************************************************************************
*  Function name    : RF_u8Get_Status
*
*  Description      : Gets the STATUS register from the RF module.
*
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
T_U8 RF_u8Get_Status()                                                                                                                                                                              
{
    return SPI_RF_u8TxCmd(RF_CMD_ReadStatus, 0, 0);
}

/***********************************************************************************************************************
*  Function name    : RF_u8Get_ObsTx
*
*  Description      : Gets the OBSERVE_TX register.
*
*  List of arguments: -
*
*  Return value     : T_U8, OBS_TX register.
*
***********************************************************************************************************************/
T_U8 RF_u8Get_ObsTx()
{
    return SPI_RF_u8TxCmd(RF_CMD_ReadReg8bit, RF_REGADDR__OBSERVE_TX, 0);
}

/***********************************************************************************************************************
*  Function name    : RF_u8Get_FifoStatus
*
*  Description      : Gets the FIFO_STATUS register
*
*  List of arguments: -
*
*  Return value     : T_U8, the FIFO_STATUS register. 
*
***********************************************************************************************************************/
T_U8 RF_u8Get_FifoStatus()
{
    return SPI_RF_u8TxCmd(RF_CMD_ReadReg8bit, RF_REGADDR__FIFO_STATUS, 0);
}

/***********************************************************************************************************************
*  Function name    : RF_vFlushTX
*
*  Description      : Empties the TX Buffer.
* 
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void RF_vFlushTX()
{
    SPI_RF_u8TxCmd(RF_CMD_TX_Flush, 0 , 0);
}

/***********************************************************************************************************************
*  Function name    : RF_vFlushRX
*
*  Description      : Empties the RX Buffer.
* 
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void RF_vFlushRX()
{
    SPI_RF_u8TxCmd(RF_CMD_RX_Flush, 0 , 0);
}

/***********************************************************************************************************************
*  Function name    : RF_vLoadTxFIFO
*
*  Description      : Sets the value(byte) to be transmitted by the RF module to the TX address.
*
*  List of arguments: T_U8 u8data
*
*  Return value     : -
*
***********************************************************************************************************************/
void RF_vLoadTxFIFO(T_U8 u8Data)
{   
    SPI_RF_u8TxCmd(RF_CMD_TX_SetPayload, 0, u8Data); 
    
    /* If TX FIFO was empty, now wait 130us (TX Setting) */
}

/***********************************************************************************************************************
*  Function name    : RF_vSetTxMode
*
*  Description      : Enters TX Mode. To transmit 1 byte, call RF_vLoadTxBuff(T_U8 u8Data).
*
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void RF_vSetTxMode()
{
    /****************/
    /* PRIM_RX = 0 */
    /* CE = 1 *****/
    /*************/
    
    T_U8 u8CfgReg;
    /* Read value from Config reg. */
    u8CfgReg = SPI_RF_u8TxCmd(RF_CMD_ReadReg8bit, RF_REGADDR__CONFIG, 0); 
    /* PRIM_RX = 0 */
    u8CfgReg = u8CfgReg & 0xFE; 
    
    /* Write new value to Config reg. */
    SPI_RF_u8TxCmd(RF_CMD_WriteReg8bit, RF_REGADDR__CONFIG, u8CfgReg); 
    
    /* CE to HIGH */
    GPIO_u8WritePortPin(PORT_A, 7, HIGH);
    
    /**************************************/
    /* Standby-II Mode if TX FIFO empty. */
    /* TX Mode if TX FIFO not empty *****/
    /* '-> 130us TX setting. ***********/
    /**********************************/
}

/***********************************************************************************************************************
*  Function name    : RF_vBeginTransmit
*
*  Description      : Use this function to Transmit data through nRF module.
*
*  List of arguments: T_U8 u8Data
*
*  Return value     : -
*
***********************************************************************************************************************/
void RF_vBeginTransmit(T_U8 u8Data)
{
    if(modeStandBy == eRF_OpMode)
    {
        RF_vFlushTX();
        
        RF_vLoadTxFIFO(u8Data);
    
        RF_vSetTxMode();
        
        eRF_OpMode = modeTX;
    }
    else if(modeRX == eRF_OpMode)
    {
        RF_vSetStandByMode();
        
        RF_vFlushTX();
                
        RF_vLoadTxFIFO(u8Data);
    
        RF_vSetTxMode();
        
        eRF_OpMode = modeTX;        
    }
    /* Poll u8IRQ_flag and check u8RF_Mode (= TX), to know when the RF module transmitted data. */
}

/***********************************************************************************************************************
*  Function name    : RF_vSetRxMode
*
*  Description      : Enters RX Mode and starts to receive any incoming data.
* 
*                   When data is received, IRQ pin is active low. (High to Low transition = Negative Edge)
*
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void RF_vSetRxMode()
{
    /****************/
    /* PRIM_RX = 1 */
    /* CE = 1 *****/
    /*************/
    
    T_U8 u8CfgReg;
    /* Read value from Config reg. */
    u8CfgReg = SPI_RF_u8TxCmd(RF_CMD_ReadReg8bit, RF_REGADDR__CONFIG, 0); 
    /* PRIM_RX = 1 */
    u8CfgReg = u8CfgReg | 0x01; 
    
    /* Write new value to Config reg. */
    SPI_RF_u8TxCmd(RF_CMD_WriteReg8bit, RF_REGADDR__CONFIG, u8CfgReg); 
    
    /* CE to HIGH */
    GPIO_u8WritePortPin(PORT_A, 7, HIGH);
    
    /* wait 130us (RX Setting) */
    //__delay_us(130);
    
    /************/
    /* RX Mode */
    /**********/
    
    /* wait for IRQ pin to go low */
}

/***********************************************************************************************************************
*  Function name    : RF_vBeginReceive
*
*  Description      : Use this function to Receive data from nRF module.
*                   Only works if nRF module is in StandBy-I Mode.
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void RF_vBeginReceive()
{
    if(modeStandBy == eRF_OpMode)
    {  
        RF_vSetRxMode();
        
        eRF_OpMode = modeRX;
    }
    /* Poll u8IRQ_flag and check u8RF_Mode (= RX), to know when the RF module received data. */
}

/***********************************************************************************************************************
*  Function name    : RF_vGetReceivedData
*
*  Description      : Gets data from nRF RX Buffer (pipe 0)  into  user's RX Buffer. 
*          
*          Sets the u8NewRX flag.
*
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void RF_vGetReceivedData()
{
    BOOL u8RxEmpty;
    
    u8RxEmpty = RF_REG_FIFOSTATUS_bit_RX_EMPTY & SPI_RF_u8TxCmd(RF_CMD_ReadReg8bit, RF_REGADDR__FIFO_STATUS, 0);
    
    /* Read data from NRF RX Buffer until there is no more data available. */
    while(FALSE == u8RxEmpty && APP_RXBUFF_DIM > u8RxBuff_Index)
    {
        au8RxBuff[u8RxBuff_Index] = SPI_RF_u8TxByteByte(RF_CMD_mask_ReadFromRX, 0x00);
        
        u8RxEmpty = RF_REG_FIFOSTATUS_bit_RX_EMPTY & SPI_RF_u8TxCmd(RF_CMD_ReadReg8bit, RF_REGADDR__FIFO_STATUS, 0);
        
        ++u8RxBuff_Index;
        u8NewRX = TRUE;   
    }
    
    /* nRF RX Buffer (pipe 0) is now empty. */
}

/***********************************************************************************************************************
*  Function name    : RF_vCheckIRQ
*
*  Description      : Checks if an interrupt occurred. If so, it clears the flags corresponding to
 *               the current operating mode.
*             
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void RF_vCheckIRQ()
{
    if(TRUE == bRF_IRQ)
    {
        bRF_IRQ = FALSE;       
        
        T_U8 u8NewStatusReg;
        
        if(modeTX == eRF_OpMode)
        {                   
            RF_vSetStandByMode();
            
            u8NewStatusReg = SPI_RF_u8TxCmd(RF_CMD_ReadStatus, 0, 0);
            /* Write 1 to clear MAX_RT bit. */
            u8NewStatusReg = RF_REG_STATUS_bit_MAX_RT | u8NewStatusReg;
            SPI_RF_u8TxCmd(RF_CMD_WriteReg8bit, RF_REGADDR__STATUS, u8NewStatusReg);             
        }
        else if(modeRX == eRF_OpMode)
        {
            /* Enter StandBy, to write (status)register. */
            RF_vSetStandByMode();
            
            RF_vGetReceivedData();

            /* clear RX_DR bit in STATUS register. 
             * (preserving the rest of the bits in STATUS register) */
            u8NewStatusReg = SPI_RF_u8TxCmd(RF_CMD_ReadStatus, 0, 0);
            /* Write 1 to clear RX_DR bit. */
            u8NewStatusReg = RF_REG_STATUS_bit_RX_DR | u8NewStatusReg;
            SPI_RF_u8TxCmd(RF_CMD_WriteReg8bit, RF_REGADDR__STATUS, u8NewStatusReg);
        }
        else if(modeStandBy == eRF_OpMode)
        {
            
        }
    }
}


/****************************************/
/* TEST FUNCTIONS **********************/
/**************************************/

void test_vReceiver()
{
    RF_vBeginReceive();
    
    if(TRUE == u8NewRX)
    {
        if(0x53U == au8RxBuff[u8RxBuff_Index-1])
        {
            GPIO_u8WritePortPin(PORT_A, 10, ~GPIO_u8ReadPortPin(PORT_A, 10));
        }
        
        u8RxBuff_Index = 0;
        u8NewRX = FALSE;
    }
}

void test_vTransmitter()
{
    RF_vBeginTransmit(0x53U);
}
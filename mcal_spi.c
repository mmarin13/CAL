/* 
 * File:   mcal_spi.c
 * Author: Cristian T. A.
 *
 * Created on August 14, 2018, 10:27 AM
 */

#include "general.h"
#include "mcal_spi.h"


/***********************************************************************************************************************
*  Function name    : SPI_RF_u8RxBuffOvf
* 
*  Description      : Returns the flag that indicates whether overflow
*                       occurred in the Receive Buffer of the SPI module.
*                    i.e. SPI module received more data than the 
*                          RX Buffer can hold.   
*                               
*  List of arguments: - 
* 
*  Return value     : BOOL, the flag (bit SPIROV)
***********************************************************************************************************************/
BOOL SPI_RF_u8RxBuffOvf()
{
    return SPI1STATbits.SPIROV;
}

/***********************************************************************************************************************
*  Function name    : SPI_RF_u8TxBuffFull
* 
*  Description      : Returns the flag that indicates whether the SPI Transmit
*                       Buffer is full or not.  
*                     
*  List of arguments: -
* 
*  Return value     : BOOL, the flag (bit SPITBF)
***********************************************************************************************************************/
BOOL SPI_RF_u8TxBuffFull()
{
    return SPI1STATbits.SPITBF;
}

/***********************************************************************************************************************
*  Function name    : SPI_RF_u8RxBuffFull
* 
*  Description      : Returns the flag that indicates whether the SPI Receive 
*                      Buffer is full or not. If the SPI module receives more 
*                       data after this flag is set,  
*                          then Overflow occurs  (  SPIROV bit is set ) 
*                    
*  List of arguments: -
* 
*  Return value     : BOOL, the flag (bit SPIRBF)
***********************************************************************************************************************/
BOOL SPI_RF_u8RxBuffFull()
{
    return SPI1STATbits.SPIRBF;
}

/***********************************************************************************************************************
*  Function name    : SPI_RF_vInit
*
*  Description      : Initializes the SPI Module to work specifically with the 
*                       nRF24L01 RF Module.
* 
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void SPI_RF_vInit()
{
    /* Reset value of STATUS register. */
    SPI1STAT = 0;
    
    
    /******************/
    /*****  SCK  *****/
    /****************/
    /* SCK set to output to pin RP1 */
    RPOR0bits.RP1R = 0b01000;       
    
    /* SCK set to input to pin RP1 */
    RPINR20bits.SCK1R = 0b00001; 
    /* Pin set as output */
    TRISBbits.TRISB1 = 0;
    /* AN3 = RP1 = Digital Pin */
    AD1PCFGLbits.PCFG3 = 1;         
    
    
    /*******************/
    /*****  MISO  *****/
    /*****************/
    /* MISO to Pin RP0 */
    RPINR20bits.SDI1R = 0;
    /* Pin set as input */
    TRISBbits.TRISB0 = 1;
    /* AN2 = RP0 = Digital Pin */
    AD1PCFGLbits.PCFG2 = 1; 
    
    
    /**********************/
    /*****  CSN(SS)  *****/
    /********************/
    /* CSN(SS) to Pin RP2 */
    RPOR1bits.RP2R = 0b01001;
    /* Pin set as output */
    TRISBbits.TRISB2 = 0;
    /* AN4 = RP2 = Digital Pin */
    AD1PCFGLbits.PCFG4 = 1;
    /* SS to HIGH */
    LATBbits.LATB2 = 1;         
    
    
    /*******************/
    /*****  MOSI  *****/
    /*****************/
    /* MOSI to Pin RP3 */
    RPOR1bits.RP3R = 0b00111;
    /* Pin set as output */
    TRISBbits.TRISB3 = 0;
    /* AN5 = RP3 = Digital Pin */
    AD1PCFGLbits.PCFG5 = 1;     
    
    
    /***************/
    /***** CE *****/
    /*************/
    /* CE to RA7 Digital OUT */
    TRISAbits.TRISA7 = 0;   
    
    
    /**********************/
    /***** Interrupt *****/
    /********************/
    /* RC6 set as input */
    TRISCbits.TRISC6 = 1;    
    
    /****************************/
    /*** SPI1CON1 description ***/
    /*************************************************
    ** 15-13 Unimplemented.
    ** 12 DISSCK = 0 : SPI clock on SCK pin is enabled
    ** 11 DISSDO = 0 : SDO pin is controlled by the module
    ** 10 MODE16 = 0 : Communication is byte-wide (8 bits)
    **  9 SMP = 0 : Input data is sampled at the middle of data output time
    **  8 CKE = 1 : Serial output data changes on transition from active clock state to Idle clock state (see bit 6)
    **  7 SSEN = 0 : (Slave mode only) SSx pin is not used by the module; pin is controlled by port function
    **  6 CKP = 0 : Idle state for clock is a low level; active state is a high level
    **  5 MSTEN = 1 : Master mode
    ** 4-2 SPRE = 110 : Secondary prescale 2:1
    ** 1-0 PPRE = 11 : Primary prescale 1:1         
    *****************************************************************/ 
    
    SPI1CON1 = 0x013B; 
    /* SPI1CON1 = (MSb) 0000 0001 0011 1011 (LSb) */
    
    /* SPI1CON1 = 0x033B;  --- 1:2 prescaler, SMP = 1 */
    /* SPI1CON1 = 0x0123;  --- 1:8 prescaler ~ 460 KHz */ 
    /* SPI1CON1 = 0x0323;  --- 1:8 prescaler, SMP = 1 */
    
    /* Framed Mode disabled */
    SPI1CON2 = 0;
    
    /* enable SPI Module */
    SPI1STAT = SPI1STAT | (1<<15); 
}

/***********************************************************************************************************************
*  Function name    : SPI_RF_vDisable
*
*  Description      : Disables the SPI Module.
*
*  List of arguments: -
*
*  Return value     : -
*
***********************************************************************************************************************/
void SPI_RF_vDisable()
{
    /* Disable SPI Module */
    SPI1STAT = SPI1STAT & (~(1<<15)); 
}

/***********************************************************************************************************************
*  Function name    : SPI_RF_u8TxByte
*
*  Description      : SPI Module transmits one Byte (8 bits)
*
*  List of arguments: T_U8 u8byte - byte to transmit
*
*  Return value     : T_U8, data received on MISO pin = nRF's Status Register. (see nRF24L01 datasheet)
*
***********************************************************************************************************************/
T_U8 SPI_RF_u8TxByte(T_U8 u8Byte)
{
    if(FALSE == SPI_RF_u8TxBuffFull()) 
    {
        /* Activate CSN(SS) */
        PORTBbits.RB2 = 0;  
        
        /* Write to Buffer, so TX can begin. */
        SPI1BUF = u8Byte;
        
        /* Just to make sure TX, RX processes are done: wait... */
        /* Wait until Buffer is empty (shifted serially out on the MOSI pin) see dsPIC33FJ datasheet, SPI Module. */
        while(SPI_RF_u8TxBuffFull());
        /* Wait until Buffer is filled with the received data, form MISO pin. */
        while(!SPI_RF_u8RxBuffFull());
        /* TX, RX processes are done. */
        
        /* Deactivate CSN(SS) */
        PORTBbits.RB2 = 1;  
    }
    
    return SPI1BUF;
}

/***********************************************************************************************************************
*  Function name    : SPI_RF_u8TxByteByte
*
*  Description      : SPI Module transmits 2 Bytes
*
*  List of arguments: T_U8 u8FirstByte, T_U8 u8SecondByte
*
*  Return value     : T_U8, data received on MISO pin.
*
***********************************************************************************************************************/
T_U8 SPI_RF_u8TxByteByte(T_U8 u8FirstByte, T_U8 u8SecondByte)
{
    if(FALSE == SPI_RF_u8TxBuffFull()) 
    {
        /* Activate CSN(SS) */
        PORTBbits.RB2 = 0;  
        
        /* Write to Buffer, so TX can begin. */
        SPI1BUF = u8FirstByte;
        
        /* Just to make sure TX, RX processes are done: wait... */
        /* Wait until Buffer is empty (shifted serially out on the MOSI pin) see dsPIC33FJ datasheet, SPI Module. */
        while(SPI_RF_u8TxBuffFull());
        /* Wait until Buffer is filled with the received data, form MISO pin. */
        while(!SPI_RF_u8RxBuffFull());
        /* TX, RX processes are done. */
        
        /* Auxiliary variable to clear the SPI Buffer, so another transmit can begin. */
        T_U8 u8Aux;
        /* clear SPI Buffer */
        u8Aux = SPI1BUF;    
        
        /* Write to Buffer, so TX can begin. */
        SPI1BUF = u8SecondByte;
        
        /* Just to make sure TX, RX process is done: wait... */
        /* Wait until Buffer is empty (shifted serially out on the MOSI pin) see dsPIC33FJ datasheet, SPI Module. */        
        while(SPI_RF_u8TxBuffFull());
        /* Wait until Buffer is filled with the received data, form MISO pin. */
        while(!SPI_RF_u8RxBuffFull());
        /* TX, RX process is done. */
        
        /* Deactivate CSN(SS) */
        PORTBbits.RB2 = 1;  
    }
    /* must return this buffer, even if it is garbage. */
    return SPI1BUF;
}

/***********************************************************************************************************************
*  Function name    : SPI_RF_u8TxCmd
*
*  Description      : Transmits a command byte to the nRF Module. (see nRF24L01 datasheet, Commands)
*                       If the command is a Read Command, then the function returns the read data.
*                       If the command is a Write Command, then the function writes the u8WrData to 
*                           the specified Register Address u8WrRdReg. 
*   
*  List of arguments: T_U8 u8Cmd, T_U8 u8WrRdReg, T_U8 u8WrData
*                       u8Cmd : RF_CMD_...
*                       
*                       u8WrRdReg : RF_REGADDR__... :
*                          byte containing the Register Address to Write/Read data to/from
*                       
*                       u8WrData : data to write to the above mentioned register.   
*                                   
*  Return value     : T_U8, data received on MISO pin.
*
***********************************************************************************************************************/
T_U8 SPI_RF_u8TxCmd(T_U8 u8Cmd, T_U8 u8WrRdReg, T_U8 u8WrData)
{
    /* If u8Cmd is not a Write Command, u8WrData is not used. */
    
    switch(u8Cmd)
    {
        case RF_CMD_ReadStatus:
            /* Returns nRF's STATUS Register */
            return SPI_RF_u8TxByte(RF_CMD_mask_NOP);
            break;
            
            
        case RF_CMD_WriteReg8bit:
            /* returns 0 */
            return SPI_RF_u8TxByteByte(RF_CMD_mask_WriteReg | u8WrRdReg, u8WrData);
            break;
            
        case RF_CMD_ReadReg8bit:
            /* Returns nRF's Register with the specified Address. */
            return SPI_RF_u8TxByteByte(RF_CMD_mask_ReadReg | u8WrRdReg, 0x00);
            break;
        

        case RF_CMD_TX_SetPayload:
            /* returns 0 */
            return SPI_RF_u8TxByteByte(RF_CMD_mask_WriteToTX, u8WrData);
            break;
   
            
            
        case RF_CMD_TX_Flush:
            /* Returns nRF's STATUS Register */
            return SPI_RF_u8TxByte(RF_CMD_mask_TX_Flush);
            break;
            
        case RF_CMD_RX_Flush:
            /* Returns nRF's STATUS Register */
            return SPI_RF_u8TxByte(RF_CMD_mask_RX_Flush);
            break;
        
        
        case RF_CMD_Activate:
            /* returns 0 */
            return SPI_RF_u8TxByteByte(RF_CMD_mask_ACTIVATE, 0x73);
            break;
            
            
        default:
            /* Returns nRF's STATUS Register */
            return SPI_RF_u8TxByte(RF_CMD_mask_NOP);
            break;      
    }
    return 0;
}

/***********************************************************************************************************************
*  Function name    : SPI_RF_u8ReadReg_5Bytes
*
*  Description      : Reads a 5 bytes-long Register from the nRF 
*
*  List of arguments: T_U8 u8RegToRead - the Register Address of a nRF register to read from.
* 
*                     T_U8 au8Result[] -  MUST be 5 bytes long. ( [5] ) 
*                       This array will contain the 5 bytes read from the 
*                           Register Address u8RegToRead
* 
*                     !!!!! au8Result[0] is the LSB of the Register.  !!!!!
* 
*  Return value     : must return the SPI Buffer. (garbage) 
*
***********************************************************************************************************************/
T_U8 SPI_RF_u8ReadReg_5Bytes(T_U8 au8Result[], T_U8 u8RegToRead)
{   
    if(FALSE == SPI_RF_u8TxBuffFull()) 
    {
        /* CSN(SS) to Low */
        PORTBbits.RB2 = 0;  
        
        /* Transmit Command to Read Register */
        SPI1BUF = RF_CMD_mask_ReadReg | u8RegToRead;  
        
        while(SPI_RF_u8TxBuffFull());
        while(!SPI_RF_u8RxBuffFull());
        
        /* Receive Status Register */
        /* It is overwritten afterwards with the first byte. */
        au8Result[0] = SPI1BUF;        
        
        
        T_U8 i;
        for(i = 0; i < 5; ++i)
        {
            /* Transmit byte to continue RX */
            SPI1BUF = 0x00;     
            
            while(SPI_RF_u8TxBuffFull());
            while(!SPI_RF_u8RxBuffFull());
            
            /* Receive byte */
            au8Result[i] = SPI1BUF; 
        }     
        
        /* CSN(SS) to High */
        PORTBbits.RB2 = 1;  
    }
    /* Clear Buffer */
    return SPI1BUF;
}

/***********************************************************************************************************************
*  Function name    : SPI_RF_u8WriteReg_5Bytes
*
*  Description      : Writes a 5 bytes-long Register of the nRF. 
*
*  List of arguments: T_U8 u8RegToWrite - the Register Address of a nRF register to write to.
* 
*                     T_U8 au8BytesToWrite[] -  MUST be 5 bytes long. ( [5] ) 
*                       This array will contain the 5 bytes to write to the 
*                           Register Address u8RegToWrite
* 
*                     !!!!! au8BytesToWrite[0] is the LSB of the Register.  !!!!!
*
*  Return value     : must return the SPI Buffer. (garbage)
*
***********************************************************************************************************************/
T_U8 SPI_RF_u8WriteReg_5Bytes(T_U8 au8BytesToWrite[], T_U8 u8RegToWrite)
{   
    if(FALSE == SPI_RF_u8TxBuffFull()) 
    {
        /* CSN(SS) to Low */
        PORTBbits.RB2 = 0;      
        
        /* Transmit Command to Write Register */
        SPI1BUF = RF_CMD_mask_WriteReg | u8RegToWrite;  
        
        while(SPI_RF_u8TxBuffFull());
        while(!SPI_RF_u8RxBuffFull());
        
        
        
        /* Variable meant to store(= clear) the SPI Buffer. */
        T_U8 u8Aux;
        
        T_U8 i;
        for(i = 0; i < 5; ++i)
        {
            /* Receive Status Register */
            u8Aux = SPI1BUF;
            
            /* Transmit byte to Write */
            SPI1BUF = au8BytesToWrite[i];    
        
            while(SPI_RF_u8TxBuffFull());
            while(!SPI_RF_u8RxBuffFull());
        }      
        
        /* CSN(SS) to High */
        PORTBbits.RB2 = 1;      
    }
    /* Clear Buffer */
    return SPI1BUF;             
}
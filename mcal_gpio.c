
#include"mcal_gpio.h"

/*******************************************************************************
*  Function name    : GPIO_u8SetPortPin
*  Description       1.Set pot pins as digital or analog,input or output                   
*  List of arguments: u8Port=port microcontroller,u8Pin=pin port microcontroller,
*                     bAD= set pin analogic or digital,bDir=select direction
*                     (input or output)
*  Return value     :  -
********************************************************************************/
void GPIO_u8SetPortPin(T_U8 u8Port,T_U8 u8Pin,BOOL bAD,BOOL bDir)
{
    switch(u8Port)
    {
        /* PORT A */ 
        case 'A':
            switch(u8Pin)
            {
                case 0:
                case 1:
                    /* set pin as analog */
                    AD1PCFGL = (AD1PCFGL &(0XFFFF-(1<<u8Pin)))|(bAD<<u8Pin);
                    /* set pin input or output  */
                    TRISA    = (TRISA &(0XFFFF-(1<<u8Pin)))|(bDir<<u8Pin);
                    break;
                case 2:
                case 3:
                case 4:
                case 7:
                case 8:
                case 9:
                case 10: 
                    /* set pin input or output  */
                    TRISA  = (TRISA &(0XFFFF-(1<<u8Pin)))|(bDir<<u8Pin);
                    break;
                default:
                    /* non-existent pin */
                    break;
            } 
            break;
        
        /* PORT B */ 
        case 'B':
            switch(u8Pin)
            {
                case 0:

                case 1:
                case 2:
                case 3:
                    /* set pin as analog */
                    AD1PCFGL = (AD1PCFGL &(0XFFFF-(1<<(u8Pin+2))))|(bAD<<(u8Pin+2));
                    /* set pin input or output  */
                    TRISB    = (TRISB &(0XFFFF-(1<<u8Pin)))|(bDir<<u8Pin);
                    break;
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                    /* set pin input or output  */
                    TRISB    = (TRISB &(0XFFFF-(1<<u8Pin)))|(bDir<<u8Pin);
                    break;

                default:
                    /* non-existent pin */
                    break;
            }
            break;
            
        /* PORT C */    
        case 'C':
            
            switch(u8Pin)
            {
                case 0:
                case 1:
                case 2:
                    /* set pin as analog */
                    AD1PCFGL = (AD1PCFGL &(0XFFFF-(1<<(u8Pin+6))))|(bAD<<(u8Pin+6));
                    /* set pin input or output  */
                    TRISC    = (TRISC &(0XFFFF-(1<<u8Pin)))|(bDir<<u8Pin);
                    break;
                case 3:       
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                    /* set pin input or output  */
                    TRISC    = (TRISC &(0XFFFF-(1<<u8Pin)))|(bDir<<u8Pin);
                    break;

                default:
                    /* non-existent pin */
                    break;
             }
             break;
             
        default :
            /* non-existent port */
            break;    
    }
}
/*******************************************************************************
*  Function name    : GPIO_u8WritePortPin
*  Description       1.Set  port pins with 1 or 0                   
*  List of arguments: u8Port=port microcontroller,u8Pin=pin port microcontroller,
*                     bValue=set pin witch value 1 or 0
*  Return value     :  - 
********************************************************************************/
void GPIO_u8WritePortPin(T_U8 u8Port,T_U8 u8Pin,BOOL bValue)
{
    switch(u8Port)
    {
        /* PORT A */    
        case 'A':
            switch(u8Pin)
            {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 7:
                case 8:
                case 9:
                case 10: 
                    /* set pin as 0 0r 1 */
                    LATA  = (LATA &(0XFFFF-(1<<u8Pin)))|(bValue<<u8Pin);
                    break;
                    
                default:
                    /* non-existent pin */
                    break;       
            } 
            break;
        
        /* PORT B */    
        case 'B':
            switch(u8Pin)
            {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                    /* set pin as 0 0r 1 */
                    LATB  = (LATB &(0XFFFF-(1<<u8Pin)))|(bValue<<u8Pin);
                    break;
                    
                default:
                    /* non-existent pin */
                    break; 
            }
            break;
         
        /* PORT C */    
        case 'C': 
            switch(u8Pin)
            {
                case 0:
                case 1:
                case 2:
                case 3:       
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                    /* set pin as 0 0r 1 */ 
                    LATC  = (LATC &(0XFFFF-(1<<u8Pin)))|(bValue<<u8Pin);
                    break;
                    
                default:
                    /* non-existent pin */
                    break;
            }
            break;
            
            
        default :
            /* non-existent port */
            break;
    } 
}   

/*******************************************************************************
*  Function name    : GPIO_u8ReadPortPin
*  Description       1.Read pin value                   
*  List of arguments: u8Port=port microcontroller,u8Pin=pin port microcontroller,
*                     
*  Return value     :  return pin value or 0xFF if the arguments are wrong
********************************************************************************/
T_U8 GPIO_u8ReadPortPin(T_U8 u8Port,T_U8 u8Pin)
{
    
    switch(u8Port)
    {
        case 'A':
            switch(u8Pin)
            {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 7:
                case 8:
                case 9:
                case 10:
                    /* return value which is loading in pin  */
                    return (PORTA>>u8Pin)&1;
                default:
                    /* non-existent pin */
                    break;
            } 
            break;
            
        case 'B':
            switch(u8Pin)
            {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                    /* return value which is loading in pin  */
                    return (PORTB>>u8Pin)&1;
                
                default:
                    /* non-existent pin */
                    break;
            }
            break;
            
        case 'C': 
            switch(u8Pin)
            {
                case 0:
                case 1:
                case 2:
                case 3:       
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                    /* return value which is loading in pin  */
                    return (PORTC>>u8Pin)&1;
                
                default:
                    /* non-existent pin */
                    break;
            } 
            break;
            
        default :
            /* non-existent port */
            break;
    } 
    
    /* return error */
    return 0xFF;
}

/*******************************************************************************
*  Function name    : GPIO_vWritePort
*  Description       1.Write  value in port                   
*  List of arguments: u8Port=port microcontroller , u16Value =value which is loading 
*                     in port
*  Return value     :  - 
********************************************************************************/
void GPIO_vWritePort(T_U8 u8Port,T_U16 u16Value)
{
    switch(u8Port)
    {
        case 'A':
            /* loading value in port */
            LATA = u16Value;
            break;
        
        case 'B':
            /* loading value in port */
            LATB = u16Value;
            break;
        
        case 'C':
            /* loading value in port */
            LATC = u16Value;
            break;
        
        default:
            /* non-existent port */
            break;
    }
}
/*******************************************************************************
*  Function name    : GPIO_s16ReadPort
*  Description       1.Read port value                   
*  List of arguments: u8Port=port microcontroller                    
*  Return value     :  return port value 
********************************************************************************/
T_U16 GPIO_u16ReadPort(T_U8 u8Port)
{
    switch(u8Port)
    {
        case 'A':
            /* return value which is loading in port */
            return PORTA;
        
        case 'B':
            /* return value which is loading in port */
            return PORTB;
        
        case 'C':
            /* return value which is loading in port */
            return PORTC;
        
        default:
            /* non-existent port */
            break;
    }
    return 0;
}
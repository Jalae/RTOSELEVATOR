/* 
 * File:   Bluetooth_Utils.h
 * Author: sean.connolly
 *
 * Created on March 12, 2013, 11:30 PM
 */

#ifndef BLUETOOTH_UTILS_H
#define	BLUETOOTH_UTILS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "portmacro.h"
#include "containers.h"

//Useful Constant Definitions:

#define BT_LOCAL_BUF_SIZE   128

#define BT_BAUD 115200
#define BT_BRG  21      //For Fpb = 40MHz
#define BT_INT_PRIORITY 0x4
#define BT_INT_SUBPRIORITY  0x1

//Synchronization Objects:
    extern xSemaphoreHandle BT_Mutex;
    extern xQueueHandle BT_RxMessageQ;
    extern xQueueHandle BT_TxMessageQ;
    extern xSemaphoreHandle Button_M;
    extern xSemaphoreHandle Button_N;
    extern xSemaphoreHandle Button_B;
    extern xSemaphoreHandle Button_V;
    extern xSemaphoreHandle Button_C;
    extern xSemaphoreHandle Button_X;
    extern xSemaphoreHandle Button_Z;


//Macro Definitions:
    
//Bluetooth UART Configuration:
    //Clear the configuration registers.
    //Clear the interrupt enables.
    //Set the Baud rate.
    //Set the mode: Off, Freeze on break, Off at Idle, Not in IRDA mode, RTS in FC mode, Tx/Rx/CTS/RTS pins in use, No wake from Sleep, No loopback, No autobaud, Rx Polarity not inverted, Baud clock standard (16x - BRGH = 0), 8-bit data/no parity, 1 stop bit
    //Set the mode bits in Status Register: Address Detect disabled, Tx interrupt held when all characters transmitted, Tx polarity not inverted, Receiver enabled, No break, Transmitter enabled, X, X, Interrupt on character receive, Address Detect mode disabled, X, X, X, Clear Overrun Error bit, X.
    //Set EIC Priority/Subpriority
    //Clear Interrupt Flags
    //Enable Receive Activity/Error Flags
    //Turn the device on
    //MODE FOR RTS/CTS: 0b0110001000000000
#define BT_UART_Init()\
U2MODE = 0x00000000;\
U2STA = 0x00000000;\
IEC1CLR = 0x00000700;\
U2BRG = BT_BRG;\
U2MODE = 0b0110000000000000;\
U2STA = 0b1001010000000000;\
IPC8bits.U2IP = BT_INT_PRIORITY;\
IPC8bits.U2IS = BT_INT_SUBPRIORITY;\
IFS1CLR = 0x00000700;\
IEC1bits.U2RXIE = 1;\
IEC1bits.U2EIE = 1;\
U2MODEbits.ON = 1




#define BT_RX_BUF_SIZE  128
    typedef struct
    {
        portBASE_TYPE m_isValid;
        char m_message[BT_RX_BUF_SIZE];
    }BT_RxMessage, *BT_RxMessagePtr;


//Function Prototypes:
    
    //ISRs:
    void BT_UART_ISR_wrapper(void);
    void BT_UART_ISR(void);

    //Service Functions:
    void BT_Write(void);        //Function controlled message.
    unsigned int BT_SendLine(const char * buffer);    //Single Line message - CR+LF parsed, will stop at first CR.
    void BT_GetChar(void);      //Returns first alpha(upper or lower case) ASCII character received.
    unsigned int BT_GetLine(BT_RxMessagePtr buffer);      //Returns all characters bufferec before <CR>





#ifdef	__cplusplus
}
#endif

#endif	/* BLUETOOTH_UTILS_H */


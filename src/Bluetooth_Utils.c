#include "Bluetooth_Utils.h"

//File-Scope Objects:
//Static declaration so they cannot be extern'd.
static BT_RxMessage g_BT_LocalBuffer;
static BT_RxMessagePtr g_BT_LocalBufferPtr = & g_BT_LocalBuffer;
static char g_BT_TxBuffer[BT_RX_BUF_SIZE];

    //Sync Objects:
    xSemaphoreHandle BT_Mutex;
    xQueueHandle BT_RxMessageQ;
    xQueueHandle BT_TxMessageQ;
    xSemaphoreHandle Button_M;
    xSemaphoreHandle Button_N;
    xSemaphoreHandle Button_B;
    xSemaphoreHandle Button_V;
    xSemaphoreHandle Button_C;
    xSemaphoreHandle Button_X;
    xSemaphoreHandle Button_Z;






//Bluetooth Activity ISR:
//NOTE: Need to add error handling for BT UART Error!
void BT_UART_ISR(void)
{
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
    //Establish the interrupt source:
    if(IEC0bits.U1EIE && IFS0bits.U1EIF)
    {
        //OHSHI-, we have a bus error!
        //This will wake the error thread later
        while(1);
    }
    if(IEC0bits.U1RXIE && IFS0bits.U1RXIF)
    {
        static unsigned int readIndex = 0;

        //We have an incoming character!
        char temp = U1RXREG;    //Not dealing with 9-bit data from this peripheral - char will be just fine.
        IFS0bits.U1RXIF = 0;
        //We're just looking for general traffic line-terminated by <CR>
        //Loop the input back out to the Bluetooth Terminal:
        U1TXREG = temp; //Do this FOR NOW - will need to make safe.

        //Look for one of the button keys:
        char test = toupper(temp);
        switch(test)
        {
            case 'M':
                xSemaphoreTakeFromISR(Button_M, &xHigherPriorityTaskWoken);
                break;
            case 'N':
                xSemaphoreTakeFromISR(Button_N, &xHigherPriorityTaskWoken);
                break;
            case 'B':
                xSemaphoreTakeFromISR(Button_B, &xHigherPriorityTaskWoken);
                break;
            case 'V':
                xSemaphoreTakeFromISR(Button_V, &xHigherPriorityTaskWoken);
                break;
            case 'C':
                xSemaphoreTakeFromISR(Button_C, &xHigherPriorityTaskWoken);
                break;
            case 'X':
                xSemaphoreTakeFromISR(Button_X, &xHigherPriorityTaskWoken);
                break;
            case 'Z':
                xSemaphoreTakeFromISR(Button_Z, &xHigherPriorityTaskWoken);
                break;
            default:

                //Looks like it's not a button...
                //Check for room:
                if(readIndex < BT_LOCAL_BUF_SIZE)
                {
                    if(temp != 0x0D)
                    {
                        if(temp == '\b')    //Look for a backspace:
                        {
                            g_BT_LocalBuffer.m_message[--readIndex] = 0x00;
                        }
                        else
                        {
                            //We have a valid character!
                            //Push it onto the buffer:
                            g_BT_LocalBuffer.m_message[readIndex] = temp;
                            g_BT_LocalBuffer.m_message[++readIndex] = 0x00;
                        }
                    }
                    else
                    {
                        //We have a line end!
                        //Message is valid at this point:
                        g_BT_LocalBuffer.m_isValid = pdTRUE;

                        //Push the local buffer onto the message queue:
                        while(xQueueSendToBackFromISR(BT_RxMessageQ, (void *)g_BT_LocalBufferPtr, &xHigherPriorityTaskWoken) != pdTRUE); //Change to if for exception handling!

                        //Reset the tracking variables & buffer for the next read:
                        readIndex = 0;
                        g_BT_LocalBuffer.m_message[0] = 0x00;
                    }
                }
                else
                {
                    //If the recent character is a line feed, it is valid - otherwise flag the message.
                    //This is so error handling can be implemented by the requesting thread.
                    //I.E.: a buffer overrun would be an annoyance to a menu system, but fatal to the command interpreter.
                    if(temp == 0x0D)
                    {
                        g_BT_LocalBuffer.m_isValid = pdTRUE;
                    }
                    else
                    {
                        g_BT_LocalBuffer.m_isValid = pdFALSE;
                    }
                    //Push the buffer onto the queue:
                    while(xQueueSendToBackFromISR(BT_RxMessageQ, (void *)g_BT_LocalBufferPtr, &xHigherPriorityTaskWoken) != pdTRUE); //Change to if for exception handling!
                    readIndex = 0;
                    g_BT_LocalBuffer.m_message[0] = 0x00;
                }

        }

    }

    if(IEC0bits.U1TXIE && IFS0bits.U1TXIF)
    {
        static portBASE_TYPE lineDone = pdTRUE;
        static unsigned int lineCount = 0;
        static unsigned int lineLength = 0;
        IEC0bits.U1TXIE = 0;
        //The write interrupt has been asserted!
        //Since this interrupt is configured to post whenever the queue is empty, we can dump 8 characters at a time.
        if((lineDone == pdTRUE) && (uxQueueMessagesWaiting(BT_TxMessageQ)!= 0))
        {
            //There is a message waiting for transmission - grab it!
            xQueueReceiveFromISR(BT_TxMessageQ, g_BT_TxBuffer, &xHigherPriorityTaskWoken);
            lineLength = strlen(g_BT_TxBuffer);
            lineDone = pdFALSE;
        }
        if(lineDone == pdFALSE)
        {
            //We already have a message - continue processing!
            int i = 0;
            for(;lineCount < lineLength && i < 4; lineCount++, i++)
            {
                U1TXREG = g_BT_TxBuffer[lineCount];
            }
            IFS0bits.U1TXIF = 0;
            //See if this line is done:
            if(lineCount == lineLength)
            {
                lineDone = pdTRUE;
                lineCount = 0;
            }
            //See if we're done - if so, disable the interrupt.
            if(lineDone == pdTRUE && uxQueueMessagesWaiting(BT_TxMessageQ) == 0)
            {
                //We're done here - disable the interrupt.
                IEC0bits.U1TXIE = 0;
            }
            else
            {
                IEC0bits.U1TXIE = 1;
            }
        }

    }
   
    //Let the kernel know we might need to wake a higher priority task:
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}



//Grab a message:
unsigned int BT_GetLine(BT_RxMessagePtr buffer)
{
    unsigned int ret = 0;

    //Reserve the device:
    xSemaphoreTake(BT_Mutex, portMAX_DELAY);
    //Wait for a read to complete:
    ret = xQueueReceive(BT_RxMessageQ, buffer, portMAX_DELAY);
    //Return the device:
    xSemaphoreGive(BT_Mutex);

    return ret;
}

unsigned int BT_SendLine(const char * buffer) //It is the responsibility of the calling function to append a carriage return.
{
    unsigned int ret = 0;

    //Reserve the device:
    xSemaphoreTake(BT_Mutex, portMAX_DELAY);

    //Push the outgoing message onto the queue:
    ret = xQueueSendToBack(BT_TxMessageQ, buffer, portMAX_DELAY);

    //Enable the interrupt
    IEC0bits.U1TXIE = 1;

    //Return the device:
    xSemaphoreGive(BT_Mutex);

    return ret;
}





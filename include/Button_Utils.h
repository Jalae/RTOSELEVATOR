/* 
 * File:   Button_Utils.h
 * Author: sean.connolly
 *
 * Created on March 12, 2013, 11:30 PM
 */

#ifndef BUTTON_UTILS_H
#define	BUTTON_UTILS_H

#ifdef	__cplusplus
extern "C" {
#endif

    //Includes:
#include <xc.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "portmacro.h"
#include "containers.h"


//External Objects:
    extern xSemaphoreHandle CN_AnyButton;
    extern xSemaphoreHandle CN_ButtonRC1;
    extern xSemaphoreHandle CN_ButtonRC2;
    extern xSemaphoreHandle CN_ButtonRD0;
    extern xSemaphoreHandle CN_ButtonRD1;
    extern xSemaphoreHandle CN_ButtonRD2;


    //Useful Constants:
#define BUTTON_PORTC_MASK     0x00000006
#define BUTTON_RC1_MASK       0x00000002    //6 - 
#define BUTTON_RC2_MASK       0x00000004    //7 - 
#define BUTTON_PORTD_MASK     0x00000007
#define BUTTON_RD0_MASK       0x00000001    //72
#define BUTTON_RD1_MASK       0x00000002    //76
#define BUTTON_RD2_MASK       0x00000004    //77

#define BUTTON_TASK_PRIORITY      1

#define DEBOUNCE_DELAY_MS   15
#define SCAN_DELAY_MS       100
    

//Macro Definitions:
#define Button_Init()\
LATCCLR = BUTTON_PORTC_MASK;\
TRISCSET = BUTTON_PORTC_MASK;\
LATDCLR = BUTTON_PORTD_MASK;\
TRISDCLR = BUTTON_PORTD_MASK;\
vSemaphoreCreateBinary(CN_ButtonRC1);\
xSemaphoreTake(CN_ButtonRC1, 0);\
vSemaphoreCreateBinary(CN_ButtonRC2);\
xSemaphoreTake(CN_ButtonRC2, 0);\
vSemaphoreCreateBinary(CN_ButtonRD0);\
xSemaphoreTake(CN_ButtonRD0, 0);\
vSemaphoreCreateBinary(CN_ButtonRD1);\
xSemaphoreTake(CN_ButtonRD1, 0);\
vSemaphoreCreateBinary(CN_ButtonRD2);\
xSemaphoreTake(CN_ButtonRD2, 0);\
vSemaphoreCreateBinary(CN_AnyButton);\
xSemaphoreTake(CN_AnyButton, 0)
    

    //Function Definitions:
    void Button_scanTask(void);



#ifdef	__cplusplus
}
#endif

#endif	/* BUTTON_UTILS_H */


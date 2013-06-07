/*System Includes...*/
#include<xc.h>
#include<sys/asm.h>

/*FreeRTOS Includes...*/
#include"ISR_Support.h"

/*Compiler Directives...*/
    .set    nomips16        /*Force use of full instruction set, regardless of project congfiguration*/
    .set    noreorder       /*Keep the assembler from optimizing the code.*/

/*Grab Externals...*/
    .extern BT_UART_ISR    /*The actual ISR to be called by the wrapper*/
    .extern BT_UART_ISR_wrapper    /*see if we can extern the prototype*/
    .extern xISRStackTop        /*Stack for ISR context switch macros*/



/*Get to it...*/
    .set    noreorder       /*FreeRTOS documentation shows this happening twice - following their lead, even if unnecessary.*/
    .set    noat
    .ent    BT_UART_ISR_wrapper

BT_UART_ISR_wrapper:

    /*API-provided macro instruction - save the interrupted task's context*/
    /*NOTE: is nesting prevented here?*/
    portSAVE_CONTEXT

    /*Call the C-code ISR*/
    jal     BT_UART_ISR
    nop     /*One for the delay branch slot.*/

    /*API-provided macro instruction - restore the interrupted task's context*/
    portRESTORE_CONTEXT

    /*All done!*/
    .end    BT_UART_ISR_wrapper



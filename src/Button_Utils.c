#include "Button_Utils.h"

//Objects:
    xSemaphoreHandle CN_AnyButton;
    xSemaphoreHandle CN_ButtonRC1;
    xSemaphoreHandle CN_ButtonRC2;
    xSemaphoreHandle CN_ButtonRD0;
    xSemaphoreHandle CN_ButtonRD1;
    xSemaphoreHandle CN_ButtonRD2;


//Function for scanning designated port pins.
//Block will scan at once, then sleep.  If there is a hit, it will sleep and
//validate the hit before continuing.
void Button_scanTask(void)
{
    //Entry Initialization:
    int PortC = 0;
    int PortCPressed = 0;
    int PortD = 0;
    int PortDPressed;

    //Task Loop:
    for(;;)
    {
        //Read the port bits:
        PortC = PORTC & BUTTON_PORTC_MASK;
        PortD = PORTD & BUTTON_PORTD_MASK;

        //Check the ports for any hits:
        if((PortC & BUTTON_RC1_MASK) && !(PortCPressed & BUTTON_RC1_MASK))
        {
            //We have a hit - wait a sec and confirm:
            vTaskDelay(DEBOUNCE_DELAY_MS/portTICK_RATE_MS);
            if(PortC & BUTTON_RC1_MASK)
            {
                //Confirmed hit - mark it!
                xSemaphoreGive(CN_AnyButton);
                xSemaphoreGive(CN_ButtonRC1);
            }
        }
        if((PortC & BUTTON_RC2_MASK) && !(PortCPressed & BUTTON_RC2_MASK))
        {
            //We have a hit - wait a sec and confirm:
            vTaskDelay(DEBOUNCE_DELAY_MS/portTICK_RATE_MS);
            if(PortC & BUTTON_RC2_MASK)
            {
                //Confirmed hit - mark it!
                xSemaphoreGive(CN_AnyButton);
                xSemaphoreGive(CN_ButtonRC2);
            }
        }
        if((PortD & BUTTON_RD0_MASK) && !(PortDPressed & BUTTON_RD0_MASK))
        {
            //We have a hit - wait a sec and confirm:
            vTaskDelay(DEBOUNCE_DELAY_MS/portTICK_RATE_MS);
            if(PortD & BUTTON_RD0_MASK)
            {
                //Confirmed hit - mark it!
                xSemaphoreGive(CN_AnyButton);
                xSemaphoreGive(CN_ButtonRD0);
            }
        }
        if((PortD & BUTTON_RD1_MASK) && !(PortDPressed & BUTTON_RD1_MASK))
        {
            //We have a hit - wait a sec and confirm:
            vTaskDelay(DEBOUNCE_DELAY_MS/portTICK_RATE_MS);
            if(PortD & BUTTON_RD1_MASK)
            {
                //Confirmed hit - mark it!
                xSemaphoreGive(CN_AnyButton);
                xSemaphoreGive(CN_ButtonRD1);
            }
        }
        if((PortD & BUTTON_RD2_MASK) && !(PortDPressed & BUTTON_RD2_MASK))
        {
            //We have a hit - wait a sec and confirm:
            vTaskDelay(DEBOUNCE_DELAY_MS/portTICK_RATE_MS);
            if(PortD & BUTTON_RD2_MASK)
            {
                //Confirmed hit - mark it!
                xSemaphoreGive(CN_AnyButton);
                xSemaphoreGive(CN_ButtonRD2);
            }
        }

        //Update the "pressed" button vars: - should sanitize these better, but
        //if they really gave a damn they'd be on change notification pins.
        PortCPressed = PortC;
        PortDPressed = PortD;

        //Wait for the next scan period:
        vTaskDelay(SCAN_DELAY_MS/portTICK_RATE_MS);
    }

    vTaskDelete(NULL);
}
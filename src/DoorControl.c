//doorshit
#include "DoorControl.h"

//Externals:
extern xSemaphoreHandle DoorOpen;	//Should make same signal as doorinterrupt?
extern xSemaphoreHandle DoorClose;

void DoorControl(void)
{
	//Init persistent members:
	unsigned char i = 0;
	unsigned char Interrupted = 0;
	int DoorState[3] = {/*LED Lib Shit*/};
	LED_B_Set(DoorState[2]);
	
	//Task Loop:
	for(;;)
	{
		xSemaphoreTake(DoorOpen, portMAX_DELAY);
		for(;i < 3 && !Interrupted; i++)
		{
			if(xSemaphoreTake(DoorClose, 1000/portTICK_RATE_MS))
			{
				Interrupted = 1;
			}
		}
		xSemaphoreTake(DoorClose, Interrupted ? 0 : 5000/portTICK_RATE_MS);
		Interrupted = 0;
		for(;i >= 0; i--)
		{
			if(xSemaphoreTake(DoorOpen, 1000/portTICK_RATE_MS))
			{
				for(;i < 3 && !Interrupted; i++)
				{
					if(xSemaphoreTake(DoorClose, 1000/portTICK_RATE_MS))
					{
						Interrupted = 1;
					}
					xSemaphoreTake(DoorClose, Interrupted ? 0 : 5000/portTICK_RATE_MS);
					Interrupted = 0;
				}
		}	
			}
	}

	vTaskDelete(NULL);
}
#include "Frequency.h"

//Externally defined variables:
extern TelemetryContainer SHM;
extern DestinationContainer Dest;
extern xQueueHandle VelocityUpdateQ;
extern xQueueHandle AccelerationUpdateQ;

//File-Scope variables
int SwitchDuration;

//FREQUENCY GOES HERE//

void Blinky(void)
{
	LED_CLR(LED_MOTOR_MASK);
	for(;;)
	{
		if(SwitchDuration >= 0)
		{
			vTaskDelay(SwitchDuration / portTICK_RATE_MS);
			LED_INV(LED_MOTOR_MASK);
		}
		else
		{
			LED_CLR(LED_MOTOR_MASK);
			vTaskSuspend(NULL);
		}
	}
	vTaskDelete(NULL);
}


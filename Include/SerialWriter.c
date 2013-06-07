//SerialWriter

#include "SerialWriter.h"

//External Items:
extern TelemetryContainer Info;

typedef enum{GD = 10, P1 = 500, P2 = 510} Floors;
typedef enum{Stopped = 0, Moving} inMotion;

//Function Definitions:
void SerialWriter(void)
{
//Initialize internals/perform configuration:
	float elevation = 0;
	float velocity = 0;
	Floors floor = 10;
	inMotion motion = 0;
	char localBuffer[128];
	
	for(;;)
	{
		taskENTER_CRITICAL;
		
		//Do the read:
		elevation = Info.Elevation;
		velocity = Info.Velocity;
		
		taskEXIT_CRITICAL;
		
		if(elevation < GD)
			floor = GD;
		else if(elevation < P1)
			floor = P1;
		else
			floor = P2;
			
		if (velocity > 0)
			motion = Moving;
		else
			motion = Stopped;

		sprintf(localBuffer, "Car currently %s near floor %s, moving $f.2'/sec, approximately $f.2' feet above ground level.\0", floor, motion, velocity, elevation);

		BT_SendLine(localBuffer);
		
		vTaskDelay(500/portTICK_RATE_MS);
	}

	vTaskDelete(NULL);
}


//Container Objects - "Containers.h":

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#ifndef CONTAINERS_H
#define CONTAINERS_H

//Telemetry Container Type Definition:
//Container links information from Frequency Generator to Writer thread for output via UART.
typedef struct
{
	float Elevation;	//Writer requires current floor, if the car is stopped/moving, current speed, and elevation.
	float Velocity;		//Can be obtained from just speed/elevation.
}TelemetryContainer, *TelemetryContainerPtr;

//Next Destination Command Container Type Definition:
//Container holds next absolute height (destination), and a semaphore for alert to change.
typedef struct
{
	int TargetElevation;
	xSemaphoreHandle DataAvailable;
}DestinationContainer, *DestinationContainerPtr;

#endif


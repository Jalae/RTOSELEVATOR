//Container Objects - "Containers.h":

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "Containers.h"
#include "DoorControl.h"
#include "Frequency.h"
#include "SerialWriter.h"

#ifndef MAIN_H
#define MAIN_H

extern DestinationContainer Dest;

extern TelemetryContainer Info;
extern DestinationContainer Dest;
extern xQueueHandle VelocityUpdateQ;
extern xQueueHandle AccelerationUpdateQ;

extern xTaskHandle SerialWriteHandle;
extern xTaskHandle ButtonScanHandle;
extern xTaskHandle FrequencyHandle;
extern xTaskHandle BlinkyHandle;

extern xSemaphoreHandle DoorOpen;
extern xSemaphoreHandle DoorClose;

#endif


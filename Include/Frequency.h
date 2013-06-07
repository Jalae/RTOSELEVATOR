//Motor Control threads

//Includes:
#include "FreeRTOS.h"
#include "task.h"
#include "LED_Utils.h"
#include "semphr.h"
#include "queue.h"

#include "ControlTask.h"
#include "Containers.h"

#ifndef FREQUENCY_H
#define FREQUENCY_H

//Useful Constants:



//Function Prototypes:
//Frequency Generator for Motor Control Output
void Frequency(void);
//Motor Control Output function
void Blinky(void);

#endif




//SerialWriter

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "BluetoothUtils.h"	//Serial device driver
#include "Containers.h"
#include <stdio.h>

#ifndef SERIALWRITER_H
#define SERIALWRITER_H

//Function Prototypes:
void SerialWriter(void);

#endif

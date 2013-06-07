

#include "Main.h"

/* Hardware configuration. */
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_2, CP = OFF, BWP = OFF
#pragma config PWP = OFF /*, UPLLEN = OFF, FSRSSEL = PRIORITY_7 */

/* Time is measured in "ticks".  The tick rate is set by the configTICK_RATE_HZ
configuration parameter (defined in FreeRTOSConfig.h).  If configTICK_RATE_HZ
is equal to or less than 1000 (1KHz) then portTICK_RATE_MS can be used to
convert a time in milliseconds into a time in ticks. */
#define mainTOGGLE_PERIOD ( 200UL / portTICK_RATE_MS )


DestinationContainer Dest;

TelemetryContainer Info;
DestinationContainer Dest;
xQueueHandle VelocityUpdateQ;
xQueueHandle AccelerationUpdateQ;

xTaskHandle SerialWriteHandle;
xTaskHandle ButtonScanHandle;
xTaskHandle FrequencyHandle;
xTaskHandle BlinkyHandle;

xSemaphoreHandle DoorOpen;
xSemaphoreHandle DoorClose;

void main(void)
{

    xTaskCreate(
        SerialWriter,
        "Serial Writer",
        configMINIMAL_STACK_SIZE,
        NULL,
        1, //change this?
        &SerialWriteHandle);

    xTaskCreate(
        Button_scanTask,
        "Button Scan",
        configMINIMAL_STACK_SIZE,
        NULL,
        3, //change this?
        &ButtonScanHandle);

    xTaskCreate(
        Frequency,
        "Button Scan",
        configMINIMAL_STACK_SIZE,
        NULL,
        1, //change this?
        &FrequencyHandle);

    xTaskCreate(
        Blinky,
        "Motor Control",
        configMINIMAL_STACK_SIZE,
        NULL,
        3, //change this?
        &BlinkyHandle);

    xTaskCreate(
        DoorControl,
        "Door Control",
        configMINIMAL_STACK_SIZE,
        NULL,
        3, //change this?
        &DoorHandle);

        vTaskStartScheduler();

}

static void prvSetupHardware(void) {
    /* Setup the CPU clocks, and configure the interrupt controller. */
    SYSTEMConfigPerformance(configCPU_CLOCK_HZ);
    mOSCSetPBDIV(OSC_PB_DIV_2);
    INTEnableSystemMultiVectoredInt();

    BT_UART_Init();
    Button_Init();
    LED_Init();
    

}


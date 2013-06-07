/*
 * File: frequency.c
 * Author: Kehnin Dyer and Sean.Connolly
 *
 * Created on June 6, 2013, 10:00 PM
 */


///////////////////////////////////////////////////////////////////////////////
//Generic units and unit conversion factors
///////////////////////////////////////////////////////////////////////////////
float FQ_UNIT = 1.0L;  //units per foot as in the maxvelocity. we are working with 1 foot units
                        //but having the expansivity for other values is good
float FQ_REZ = 10.0L;  //The number of milliseconds per computation slice

//conversion factors
    //Where the unit conversions come from:
    //for velocity
    // FQ_UNIT t   Vel  ft    1    s    FQ_REZ ms   FQ_UNIT*Vel*FQ_REZ tickunits (a distance)
    // --------- X -------- X ------- X --------- = ------------------ ---------
    // 1      ft   1     s    1000 ms   1   slice   1000               slice (a time)
    //
    //for accel
    // FQ_UNIT t   Accel ft   1        s^2   FQ_REZ^2 ms^2   FQ_UNIT*Accel*FQ_REZ^2 tickunits (a distance)
    // --------- X -------- X ------------ X ------------- = ---------------------- ---------
    // 1      ft   1    s^2   1000000 ms^2   1     slice^2    1000000               slice^2   (a time)
    //
    //for tickFlashtime * 2
    // / TPSCurVel Tick   1   Slice   1       ft     1000 ms   1 Hz    \ ^-1     1000ms     10*FQ_REZ*FQ_UNIT ms
    // | -------------- X --------- X ------------ X ------- X -------  |    s X ------- = ---------------------- period
    // \         Slice    FQ_REZ ms   FQ_UNIT Tick         s   10 ft/s /              s          TPSCurVel
    //
float TPSSperFPSS = FQ_UNIT*FQ_REZ*FQ_REZ/1000000.0L;
float TPSperFPS   = FQ_UNIT*FQ_REZ/1000.0L;
float MSperTPS    = (5.0L * FQ_UNIT * FQ_REZ);


//These are modifyable "constants"
int maxVelocity  = 40;  //The max velocity of the lift in units/s
int Accel        = 2 ;  //The acceleration of the lift in units/ss

//working variables
int   DestFt = 0; //updates from control

float CurFt  = 0.0L; //internal state height
float CurVel = 0.0L;

int SwitchDuration = -1; //this is for blinky

int Dir    = 0; //Pos 1 for up, neg 1 for down
int Emergency = 0;

//calculated generic unit values
float TPSAccel       = 0.0L;
float TPSMaxVelocity = 0.0L;
float TPSCurVelocity = 0.0L;
float TDistance      = 0.0L;
float TDestination   = 0.0L;

float TStopPoint = 0.0L;


//Externally defined variables:
extern TelemetryContainer Info;
extern DestinationContainer Dest;
extern xQueueHandle VelocityUpdateQ;
extern xQueueHandle AccelerationUpdateQ;

extern xTaskHandle BlinkyHandle;

int vCalculateFrequency()
{
    int toofast;
    
    if( xSemaphoreTake( Button_B, (portTickType)0 ) == pdTRUE )
    {
        Emergency = 1;
        xSemaphoreGive( Button_B );
    }
    if( xSemaphoreTake( Button_N, (portTickType)0 ) == pdTRUE )
    {
        Emergency = 0;
        xSemaphoreGive( Button_B );
    }
    if(Emergency)
        DestFt = 0; //oh god


    if(uxQueueMessagesWaiting( VelocityUpdateQ ))
    {
        xQueueReceive(VelocityUpdateQ, &(maxVelocity), (portTickType) 0);
    }
    if(uxQueueMessagesWaiting( AccelerationUpdateQ ))
    {
        xQueueReceive(AccelerationUpdateQ, &(Accel), (portTickType) 0);
    }

    SwitchDuration = TPSCurVelocity? (int)MSperTPS/(TPSCurVelocity) : 0;
    TPSAccel       = TPSSperFPSS * Accel;
    TPSMaxVelocity = TPSperFPS * maxVelocity;

    TDestination = FQ_UNIT * DestFt; // this _shouldn't_ change, but it can... as in EMERGENCY STOP
    TDistance = TPSCurVelocity + TDistance;
    CurFt = TDistance/FQ_UNIT;

    //due to float use == is bad so lets add some wiggle room
    Dir = (DestFt > CurFt + .001L) ? 1 : ((DestFt < CurFt - .001L) ? -1 : 0 );
        //NOTE: there is a chance that a change in accel or max velocity will cause unintended behavior,
        // one instance of that is changeing accel so that stoping in time is impossible, this will hopefully
        // catch that and cause an abrupt stop.
        //(it is possible that it will move too fast and jump past the stop window)

    //to calculate distance to stop:
    // E1)  Df = Di + Vi*T+.5AT^2
    // E2)  Vf = Vi + AT
    // Df = distance, Di = 0, Vi = 0, A = Accel, Vf = maxVelocity
    // solve E2 for T, Plug in T in E1 to solve...
    // Distance = .5 maxVelocity^2 / Accel
    //units are determined by vel and accel, and since we want Ticks TPS and TPSS it is...
    TStopPoint = TDestination - Dir*((.5 * TPSCurVelocity * TPSCurVelocity) / TPSAccel);

    //if we are past the stop point in the next slice, time to slow down.
    if(Dir*TDistance >= Dir*TStopPoint)
        Dir = -Dir; //invert that direction to decelerate to 0.

    //is the new magnitude of velocity greater than the max
    toofast = ( (((TPSCurVelocity + Dir*TPSAccel) > 0) * (TPSCurVelocity + Dir*TPSAccel)) > TPSMaxVelocity );
    TPSCurVelocity = Dir ? (toofastsean connolly ? Dir*TPSMaxVelocity:(TPSCurVelocity + Dir*TPSAccel)):0;

    CurVel = TPSCurVelocity/TPSperFPS;

    //write the data for serialwriter
    taskENTER_CRITICAL;
    Info.Velocity  = CurVel;
    Info.Elevation = CurFt;
    taskENTER_CRITICAL;

    return CurVel;
}

void Frequency(void)
{
    int rolling;
    while(1)
    {
        SwitchDuration = -1;
        if( xSemaphoreTake( Dest.DataAvailable , (portTickType) portMAX_DELAY ) == pdTRUE )
        {
            SwitchDuration = 0;
            DestFt = Dest.TargetElevation;
            vTaskResume( BlinkyHandle );

            do
            {
                rolling = vCalculateFrequency(); // returns the speed, so when it's done, it's done.
                vTaskDelay( (portTickType) FQ_REZ/portTICK_RATE_MS )
            }while(rolling);
            xSemaphoreGive( Dest.DataAvailable );
        }
    }
}


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

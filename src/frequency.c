/*
 * File: frequency.c
 * Author: Kehnin Dyer
 *
 * Created on June 6, 2013, 10:00 PM
 */

///////////////////////////////////////////////////////////////////////////////
//Generic units and unit conversion factors
///////////////////////////////////////////////////////////////////////////////
int FQ_UNIT = 1;  //units per foot as in the maxvelocity. we are working with 1 foot units
                        //but having the expansivity for other values is good
int FQ_REZ = 10;  //The number of milliseconds per computation slice

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
float TPSSperMPSS = FQ_UNIT*FQ_REZ*FQ_REZ/1000000;
float TPSperMPS   = FQ_UNIT*FQ_REZ/1000;
float MSperTPS    = (5 * FQ_UNIT * FQ_REZ);


//These are modifyable "constants"
int maxVelocity  = 40;  //The max velocity of the lift in units/s
int Accel        = 2 ;  //The acceleration of the lift in units/ss

//working variables
int   DestFt = 0; //updates from control

float CurFt  = 0; //internal state height
int   flashtime_ms = -1; //this is for blinky

int   Dir    = 0; //might not be used?

//calculated generic unit values
float TPSAccel       = 0.0L;
float TPSMaxVelocity = 0.0L;
float TPSCurVelocity = 0.0L;
float TDistance      = 0.0L;
float TDestination   = 0.0L;

void vCalculateFrequency()
{

    //TODO
    //check queue for value, if exists update maxVelocity and Acceleration


    flashtime_ms   = (int)MSperTPS/(TPSCurVelocity);
    TPSAccel       = TPSSperMPSS * acceleration;
    TPSMaxVelocity = TPSperMPS * maxVelocity;

    TDistance      = 
}

void vtaskFrequency()
{

    //wait for lock
        tDestination
    //to calculate distance to stop:
    // E1)  Df = Di + Vi*T+.5AT^2
    // E2)  Vf = Vi + AT
    // Df = distance, Di = 0, Vi = 0, A = Accel, Vf = maxVelocity
    // solve E2 for T, Plug in T in E1 to solve...
    // Distance = .5 maxVelocity^2 / Accel
    //units are determined by vel and accel, and since we want Ticks TPS and TPSS it is
}

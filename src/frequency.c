/*
 * File: frequency.c
 * Author: Kehnin Dyer
 *
 * Created on June 6, 2013, 10:00 PM
 */

int maxVelocity  = 40;  //The max velocity of the lift in units/s
int acceleration = 2 ;  //The acceleration of the lift in units/ss

#define FQ_UNIT 1
    //units per foot as in the maxvelocity. we are working with 1 foot units
    //but having the expansivity for other values is good
#define FQ_REZ 10
    //The number of milliseconds per computation slice

portTickType MaxTicks   = 0;
portTickType AccelTicks = 0;
portTickType CurSpeed   = 0;


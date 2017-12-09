/**
 *
 * Math and maxim (fact-provided) library for `jeep` system. Allows math
 * operation for vehicle and automobile standards.
 *
 * maxim.h
 *
 * Copyright (c) 2017 Halis Duraki. All rights reserved.
 *
 * Author:
 * Halis Duraki <duraki@linuxmail.org> 
 *
 **/

#include <math.h>

#define PI 3.14159265358979
    
float 
engine_capacity(float bore, float stroke, int cylinder)
{
    return (pow(bore, 2) * stroke * PI) / 4000 * cylinder;  
}

float
tq_output(float it, float or)
{
    return (it * or);
}

float
tq_to_hp(float tq, float mxrpm)
{
    return tq * (mxrpm / 60 * (PI * 2));
}

float
hp_to_tq(float power, float mxrpm)
{
    return power / (mxrpm / 60 * (PI * 2));
}

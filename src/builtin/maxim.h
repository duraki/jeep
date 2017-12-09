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

/**
 * Lets get total residual error probability on our CAN bus. We can get these by 
 * using power of 10 on whole nxp (a)`-b = 1/(a`b).
 *
 * @mer int message error rate
 * @return float err probability
 */
float
BUS_err_probability(int mer)
{
    return (mer * 4.7 * (1 / (pow(10, 11))));
}

float mod(float a, float N) {return a - N*floor(a/N);} // return in range [0, N)
    
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

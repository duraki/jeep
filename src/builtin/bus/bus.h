/**
 *
 * Bus module for working with main interface. Should allow various helper
 * methods on base Bosch standards. Message manipulation enums.
 *
 * Should be supportable by API.
 *
 * bus.h
 *
 * Copyright (c) 2017 Halis Duraki. All rights reserved.
 *
 * Author:
 * Halis Duraki <duraki@linuxmail.org> 
 *
 **/

#ifndef JEEP_BUS
#define JEEP_BUS

#define BUS_TYPE "DOMINANT", "RECESSIVE"

#define BUS_FRAME "DATA" /* data from transmitter to receivers */,\ 
                  "REMOTE" /* data from bus unit */,\
                  "ERROR" /* data from any unit on detected error  */,\
                  "OVERLOAD" /* frame to provide extra delay between frames */

#define ZERO 0

float
errors()
{
    return BUS_err_probability(ZERO);
}

#endif

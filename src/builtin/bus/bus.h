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

#include <limits.h>
#include <linux/can.h>
#include <linux/can/raw.h>

#ifndef JEEP_BUS
#define JEEP_BUS

#define MODULE "BUS"

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

int
calculate_crc_sequence(struct can_frame l_frame, struct can_frame curr_frame, int seq);

void
__prepare_for_ack()
{

}

#endif

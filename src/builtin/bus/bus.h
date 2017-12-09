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
calculate_crc_sequence(struct can_frame l_frame, struct can_frame c_frame, int seq)
{
    /**
     * Since frame on bus are transferred MSB first, we are required to separate
     * CRC field from it. CRC field on frame exists and contain sequence and
     * separator.
     *
     * Once message is acked, another frame is divided.
     *
     * Frame check sequence is derived from a crc best suited for frames with
     * bit counts less than 127 bits, polynomials on bch.
     *
     * SOF.
     * =======================================================================
     * data fld | crc field                         |ACK ->
     * =======================================================================
     * ctrl fld |                            crcsplt|ACK ->
     * =======================================================================
     *                  |crc seq|
     *                  ======================================================
     *
     * sizeof[(data fld) || (ctrl fld)] < CHAR_BIT * s (15) | 127
     *
     *
     *
     * First we need to get polynomial through the coefficients by extracting or
     * unloading bit stream of SOF, ARBID, CONTROL FIELD, DATA FIELD and for the
     * 15 lowest coefficents by 0. 
     *
     * Coefficents are are calculated using modulo-2. Once ready ...
     *
     * mod(data fld, 15)
     *
     * This polynomial is then devided by generator polynomial:
     *
     * X`15 + X`14 + X`10 + X`8 + X`7 + X`4 + X`3 + 1
     *
     * Once again, a beautiful crc sequence is in the reminder of above
     * polynomial. 
     *
     * Same function in BHC is possible through shift register `crc_rg` val 14,0. 
     *
     * --
     * CRC_RG = 0; # => Initialize shift register
     *
     * do { # => Do until got crc seq start
     *      CRCNXT       = NXTBIT XOR CRC_RG(14) # => eq: nxtbit ⊕ crc_rg
     *      CRC_RG(14:1) = CRC_RG(13:0); # => shift left by one bit
     *      CRC_RG(0)    = 0;
     *
     *      if CRCNXT then 
     *          CRC_RG(14:0) = CRC_RG(14:0) XOR (0x4599)
     *      endif
     *
     * } until 
     *  > CRC SEQUENCE || EIC
     *
     * The above explains how CRC_RG at the end contains the CRC sequence. 
     *
     * Once everything is ready, prepare the operands for CRC splitter or
     * delimiter. Delimiter can be found at the LSB of the sequence. It consist
     * a single `RECESSIVE` bit.
     *
     * @see BUS_TYPE
     *
     *
     * Finally __prepare_for_ack() to initialize data before EOF.
     *
     */
}

#endif

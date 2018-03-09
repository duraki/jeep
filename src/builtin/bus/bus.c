#include "bus.h"

/**
 * @l_frame can_frame last frame
 * @curr_frame can_frame current frame
 * @seq int
 */
int
calculate_crc_sequence(struct can_frame l_frame, struct can_frame curr_frame, int seq)
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
     */
    struct can_frame fd;
    fd = curr_frame;

    if (sizeof(fd) < 15)
        say(MODULE, "Current frame bitcount is less than 127 bits. I'll skip it.");

    /**
     *
     * First we need to get polynomial through the coefficients by extracting or
     * unloading bit stream of SOF, ARBID, CONTROL FIELD, DATA FIELD and for the
     * 15 lowest coefficents by 0. 
     *
     * Coefficents are are calculated using modulo-2. Once ready ...
     *
     * mod(data fld, 15)
     */
    canid_t canid; /** can id **/
    canid = fd.can_id;

    int f_len; /** can frame length **/

    /**
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
     * do { # => Do until got crc seq start (pseudo)
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
     * In order to carry out CRC, polynomial to be divided is defined as the
     * polynomial a = a, but the thing is there are different CAN layers @
     * ISO-*. What we have to do is defined netd of tty/ttl.
     *
     * Finally __prepare_for_ack() to initialize data before EOF.
     *
     */
}

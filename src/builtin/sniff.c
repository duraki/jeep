/**
 *
 * Dump CAN traffic and show diff.
 * sniff.c 
 *
 * Copyright (c) 2017 Halis Duraki. All rights reserved.
 *
 * Author:
 * Halis Duraki <duraki.halis@nsoft.com> 
 *
 **/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <ctype.h>
#include <libgen.h>
#include <time.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <net/if.h>

#include <linux/can.h>
#include <linux/can/bcm.h>

static const char * const jeep_sniff_usage[] = {
    N_("jeep sniff [device]",
    N_("jeep sniff [device] |  --verbose"),
    NULL

}

enum sniff_type { BASIC, MIXED };
static const char *sniff_type_names[] = {
    N_("basic"), N_("mixed"), NULL
};

static void parse_args(const char **argv, const char *prefix)
{

}

int cmd_sniff(int argc, char **argv, const char *prefix)
{
    return 0;
}

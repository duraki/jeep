/**
 *
 * Dump CAN traffic and show diff in a table. Includes colored output and
 * speicific sort or filter.
 *
 * sniff.c 
 *
 * Copyright (c) 2017 Halis Duraki. All rights reserved.
 *
 * Author:
 * Halis Duraki <duraki@linuxmail.org> 
 *
 *           ____________ ____ 
 *          / /__  /__  // __ \
 *     __  / / /_ < /_ </ /_/ /
 *    / /_/ /___/ /__/ / ____/ 
 *    \____//____/____/_/      
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
#include <ncurses.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/uio.h>

//#include <linux/if.h>
#include <net/if.h>
#include "linux/can.h"
#include "linux/can/bcm.h"

#include "api.h"
#include "ui.h"
#include "util.h"

#define MODNAME     "sniff"
#define MODVERSION  "1.0.0"

/* time defults */
#define TIMEOUT 500 /* in 10ms */
#define HOLD    100 /* in 10ms */
#define LOOP     20 /* in 10ms */

#define PAD     50

static 
const char * jeep_sniff_usage[] = {
    "[jeep] sniff [device]",
    "[jeep] sniff [device]",
    NULL
};

enum sniff_type { BASIC, MIXED };

static 
const char *sniff_type_names[] = {
    "basic", "mixed", NULL
};

struct sniff {
    int     flags;
    long    hold;
    long    timeout;
    struct  timeval     laststamp;
    struct  timeval     currstamp;
    struct  can_frame   last;
    struct  can_frame   current;
    struct  can_frame   marker;
    struct  can_frame   notch;
} sniftab[2048];

static const char *thead[] = {
    "TIMEOUT", "ARBID",
    "DATA", "ASCII", NULL
};

int ifindex    = 0;
char *interface = 0;

int row, col; /* term size */

int 
main(int argc, char *argv[])
{
    int index    = 0;
    char *device;

    ui_win(&row, &col);

    say(MODNAME, "Buffering device default index to zero.");
    strcpy(device, "*");

    if (argc > 1) {
        int index = interface_index(argv[1]);
        strcpy(device, argv[1]); 
    } 

    char tty[50];
    sprintf(tty, "%s:%d", device, index);

    ui_module(MODNAME, MODVERSION, tty, row, col);
    ui_help(default_help_msg);
    ui_footer(sniff_type_names[0]);
    ui_table(thead, sizeof(thead), &row, &col);

    initialize(index);

    ui_freeze();
    return 1;
}

void
initialize(int device_in)
{
    static struct ifreq ifr;
    static struct sockaddr_ll sl;
    static struct can_frame frame;
    
    int i;

    time_t t_1, t_2;
    char   delta[50];

    int sock = create_socket();
    sl       = bind_socket(device_in, &sock, sl);     

    while (1) {
        t_1 = time(0);
        frame = read_socket(sock);
        t_2 = time(0);

        sniff(i++, frame, t_1, t_2);
    }
}

void 
sniff(int i, struct can_frame frame, time_t t_last, time_t t_curr)
{
    char delta[10];
    char f_id[50];

    snprintf(delta, 10, "%3f", t_last - t_curr);
    snprintf(f_id, 50, "%d", frame.can_id);

    const char *r[] = {
       delta, 
       f_id
    };

    ui_table_row(r, i, row, col);
}

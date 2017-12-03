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
 * Halis Duraki <duraki.halis@nsoft.com> 
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

#include <linux/if.h>
#include <linux/can.h>
#include <linux/can/bcm.h>

#include "api.h"
#include "ui.h"
#include "util.h"

#define MODNAME "sniff"
#define MODVERSION "1.0.0"

#define INTERFACE "*"

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
    int flags;
    long hold;
    long timeout;
    struct timeval laststamp;
    struct timeval currstamp;
    struct can_frame last;
    struct can_frame current;
    struct can_frame marker;
    struct can_frame notch;
} sniftab[2048];

char *interface = INTERFACE;

int row, col; /* term size */

int sniff()
{
    int s;
    struct can_frame frame;
    int size, i;
    int ifindex; /* ifr index */
    int ct_sniff = 4;
    int cr = row/4;

    static struct ifreq ifr; /* instance */
    static struct sockaddr_ll sl; 

    initscr();
    getmaxyx(stdscr, row, col);

    attroff(COLOR_PAIR(1));
    
    s = create_socket();
    if (s < 0)
        perror("socket");

    if (strcmp(interface, INTERFACE) == 0)
        ifindex = 0; /* any */

    if (strcmp(interface, INTERFACE) != 0) {
        strcpy(ifr.ifr_name, interface);
        ifindex = ifr.ifr_ifindex; 
    }

    if (bind_socket(ifindex, s, sl) != 0) {
        mvprintw(col/2, (row-50)/2, "Error while binding socket.");
        refresh();
        endwin();
    }

    while (1) {

        if ((size = read(s, &frame, sizeof(struct can_frame))) < 0) {
            flog("Can't read from device.");
            return 1;
        } else if (size < sizeof(struct can_frame)) {
            flog("Can't detect valid frame structure.");
            return 1;
        } else {
            //fprintf(stdout, "Its working!");
        }

    }

    return 1;
}

int
print_table()
{
    initscr();
    getmaxyx(stdscr, row, col);

    attron(COLOR_PAIR(1) | A_BOLD);

    static const char *table[] = {
        "TIMEOUT", "ARBID",
        "DATA", "ASCII", NULL
    };

    int ct = 2;
    int cr = row/4;

    mvprintw(ct, 0, table[0]);
    mvprintw(ct, (cr+strlen(table[0])), table[1]);
    mvprintw(ct, (cr+cr+cr+strlen(table[1])), table[2]);
    mvprintw(ct, col-strlen(table[3]), table[3]);

    refresh();
}

int
print_footer()
{
    int x, y;
    initscr();
    getmaxyx(stdscr, y, x);

    attroff(A_BOLD);
    mvprintw(y-1, 0, "mode: %s", sniff_type_names[0]);

    refresh();
}

int
init_win()
{
    initscr();
    int x, y;
    getmaxyx(stdscr, y, x);

    row = x;
    col = y;
}

int
init_module()
{
    initscr();

    flog("Initializing module.");
    print_table();
    print_footer();

    sniff();

    refresh();
    //getch();
}

int 
main(int argc, char *argv[])
{
    initscr(); /* must always init first */
    init_win();

    ui_module(MODNAME, MODVERSION, INTERFACE, row, col);
    ui_help(default_help_msg);
    init_module();

    ui_freeze();
    endwin();
}


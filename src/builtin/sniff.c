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

#define MODNAME "sniff"
#define MODVERS "1.0.0"

#define INTERFACE "*"

/* time defults */
#define TIMEOUT 500 /* in 10ms */
#define HOLD    100 /* in 10ms */
#define LOOP     20 /* in 10ms */

static 
const char * const jeep_sniff_usage[] = {
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

int
init_screen()
{
    initscr();
    int x, y; /* visible area */
    getmaxyx(stdscr, y, x);

    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);

    attron(COLOR_PAIR(1) | A_BOLD); /* set style */

    /* print modname */
    char modname[50];
    sprintf(modname, "modname: [%s]", MODNAME);
    printw(modname);

    /* print modversion */
    char modversion[50];
    sprintf(modversion, "modversion: [%s]", MODVERS);
    mvprintw(0, (x-strlen(modname))/2, modversion);

    /* print interface */
    char inf[50];
    sprintf(inf, "interface: [%s]", INTERFACE);
    mvprintw(0, x-strlen(inf), inf);

    attroff(COLOR_PAIR(1));

    move(y+3, x);
    printw("\n%-10s %-10s %-30s %-30s\n\n", 
            "TIMEOUT", 
            "ARBID",
            "DATA", 
            "ASCII",
            "BIN",
            "LAST CHANGE");


    attroff(A_BOLD);
    mvprintw(y-1, 0, "mode: %s", sniff_type_names[0]);

    refresh();
    noecho();
    getch();
    endwin();

    return 1;
}

int 
init_sniff()
{
    int s;
    struct can_frame frame;
    int size, i;
    int ifindex; /* ifr index */
    static struct ifreq ifr; /* instance */
    static struct sockaddr_ll sl; 

    initscr();
    int x, y; /* visible area */
    getmaxyx(stdscr, y, x);

    s = create_socket(); /* use api to create a raw socket */
    if (s < 0)
        perror("socket");

    if (strcmp(interface, INTERFACE) == 0)
        ifindex = 0; /* listen to all interfaces */

    if (strcmp(interface, INTERFACE) != 0)
        strcpy(ifr.ifr_name, interface);
        ioctl(s, SIOCGIFINDEX, &ifr);
        ifindex = ifr.ifr_ifindex;

    if (bind_socket(ifindex, s, sl) > 0)
        wprintw(y/2, (x-50)/2, "Error while binding socket.");
        refresh();
        endwin();
        getch();

    while (1) {
        
    }

    return 1;
}

int 
main(int argc, char *argv[])
{
    init_screen();
    init_sniff();
}


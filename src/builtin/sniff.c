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
#include <net/if.h>

#include <linux/can.h>
#include <linux/can/bcm.h>

#include "api.h"

#define MODNAME "sniff"
#define MODVERS "1.0.0"

#define INTERFACE "*"

static const char * const jeep_sniff_usage[] = {
    "[jeep] sniff [device]",
    "[jeep] sniff [device]",
    NULL
};

enum sniff_type { BASIC, MIXED };

static 
const char *sniff_type_names[] = {
    "basic", "mixed", NULL
};

int
init_screen()
{
    initscr();
    int x, y; /* visible area */
    getmaxyx(stdscr, y, x);

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);

    attron(A_BOLD); /* set style */

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

    attroff(A_BOLD);
    mvprintw(y-1, 0, "mode: %s", sniff_type_names[0]);

    refresh();
    noecho();
    getch();
    endwin();

    return 1;
}

int 
main(int argc, char *argv[])
{
    init_screen();
}


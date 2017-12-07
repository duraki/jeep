#ifndef JEEP_UI
#define JEEP_UI

#include "util.h"

const char *default_help_msg = "Press `q` to quit.";

/*
 * Display version name and module plus interface attached to it.
 *
 * name: module name
 * version: module version
 * iface: interface
 * row, col: /
 */
int 
ui_module(char *name, char *version, char *iface, int row, int col)
{
    initscr();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);

    say("UI", "Initializing UI for module info");

    //attron(COLOR_PAIR(1) | A_BOLD); /* set style */

    /* print modname */
    char modname[50];
    sprintf(modname, "modname: [%s]", name);
    printw(modname);

    /* print modversion */
    char modversion[50];
    sprintf(modversion, "modversion: [%s]", version);
    mvprintw(0, (row-strlen(modname))/2, modversion);

    /* print interface */
    char inf[50];
    sprintf(inf, "interface: [%s]", iface);
    mvprintw(0, row-strlen(inf), inf);

    refresh();
}

int
ui_freeze()
{
    char *c;
    while ((c = getchar()) != 'q') {
        // # => Skip non-known chars
    }

    endwin();
}

int
ui_help(char *msg)
{
    int x, y; /* visible area */
    initscr();
    getmaxyx(stdscr, y, x);
    attroff(A_BOLD); 
    mvprintw(y-2, 0, "%-20s\n", msg);
}

int
ui_win(int *row, int *col)
{
    int x, y;
    initscr();
    getmaxyx(stdscr, y, x);

    *row = x;
    *col = y;
}

int
ui_footer(char *msg)
{
    int x, y;
    initscr();
    getmaxyx(stdscr, y, x);

    attroff(A_BOLD);
    mvprintw(y-1, 0, "f: %s", msg);

    refresh();
}

int
ui_table(char *th[], int sth, int row, int col)
{
    int x, y;
    initscr();
    getmaxyx(stdscr, row, col);

    attron(COLOR_PAIR(1) | A_BOLD);

    int ct = 2;
    int cr = row/4;

    mvprintw(ct, 0, th[0]);
    mvprintw(ct, (cr+strlen(th[0])), th[1]);
    mvprintw(ct, (cr+cr+cr+strlen(th[1])), th[2]);
    mvprintw(ct, col-strlen(th[3]), th[3]);

    refresh();
}

#endif

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


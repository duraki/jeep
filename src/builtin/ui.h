/*
 * Display version name and module plus interface attached to it.
 *
 * name: module name
 * version: module version
 * iface: interface
 * row, col: /
 */
int ui_module(char *name, char *version, char *iface,
        int row, int col)
{
    initscr();
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);

    attron(COLOR_PAIR(1) | A_BOLD); /* set style */

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


/**
 *
 * This is `brain` of the jeep system. The initial command parsing and api lay
 * here. One may read this code for a better understanding when writing a
 * module. Providing builtin system to search in/ext sub-cmds, this also
 * transfer and collect a log.
 *
 * jeep.c 
 *
 * Copyright (c) 2017 Halis Duraki. All rights reserved.
 *
 * Author:
 * Halis Duraki <duraki.halis@nsoft.com> 
 *
 **/

#include <string.h>

#include "builtin.h"
#include "jeep.h"
#include "builtin/api.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <string.h>
#include <ncurses.h>

int print_usage()
{
    const char usage [] = {
        "jeep \t --devices [-info]\n"
        "jeep \t --about --version\n\n"
        "jeep \t command [--d=xxxxxx] | device | -info\n"
        "jeep \t command [--l=custom] | list\n\n"
    };

    fprintf(stderr, "%s", usage);

    return -1;
}

int
win_devices(struct device_i *dev_l)
{
    initscr();
    int x, y; /* visible area */
    getmaxyx(stdscr, y, x);
    attron(A_BOLD); 

    printw("%-10s %-30s %-10s %-20s %-10s %-10s\n\n", 
            "[#]", 
            "DEVICE",
            "INBYTES", 
            "INPACKETS",
            "OUTBYTES",
            "OUTPACKETS");

    mvprintw(y-1, 0, "%-10s\n", "jeep | [device list]");

    attroff(A_BOLD);
    for (int i = 0; i < 30; i++) {
        y = i+1;
        if (ldevice[i].id != 0) {
            mvprintw(y, 0, "%-10d %-30s %-10d %-20d %-10d %-10d\n", 
                    ldevice[i].id, 
                    ldevice[i].ifr.ifname,
                    ldevice[i].ifr.r_bytes,
                    ldevice[i].ifr.r_packets,
                    ldevice[i].ifr.t_bytes,
                    ldevice[i].ifr.t_packets);
        }
    }

    refresh();
    getch();
    endwin();
}

int 
main(int argc, char *argv[]) 
{
    char *pname = argv[0];  /* parent proc name */
    char *cmd;              /* command to exec */

    char *device;           /* device uuid or mounpoint */
    bool about = false;     /* is in about */
    bool list = false;      /* is in list */

    int getopt_ret, option_index;
    static struct option long_options[] = {
               {"device",  required_argument, 0, 'd'},
               {"list",  optional_argument, 0, 'l'},
               {0, 0, 0, 0}};

    get_art();
    init_builtin();         /* init builtin cmds */
    init_custom();          /* init custom cmds */

    if (argv[1]) {
        char *cmd = argv[1];

        if (strcmp(cmd, "--devices") == 0) { /* todo: -info */
            printf("[*] Searching for connected devices ...\n"); 
            struct device_i *dev_l = get_interface_list();
            win_devices(dev_l);
        }

        if (strcmp(cmd, "--about") == 0) {
            printf("[o_o] `jeep`\t - penetration testing framework for vehicle systems\n"); 
            return 1;
        }

        if (strcmp(cmd, "--version") == 0) {
            get_version();
            return 1;
        }

        if (strcmp(cmd, "--list") == 0) {
            list_builtin();
            return 0;
        }

        printf("[*] Getting command module: %s\n", cmd);

        if (search_cmd(cmd) == -1) {
            printf("[x] Command not available in built-in system.\n[*] Searching in custom modules.\n");
            return 1;
        }

    }

    if (argc <= 1) { /* no args */
	print_usage();
    }

    while (1) {
        getopt_ret = getopt_long( argc, argv, "d:l::",
                                  long_options,  &option_index);
        if (getopt_ret == -1) break;

        switch(getopt_ret)
        {
	    printf("\n");
            case 0: break;
            case 'd':
                printf("[*] Device set to: %s\n", optarg); break;
                break;
            case 'l':
                if (!optarg) { /* --l */
                    list_builtin();
                    return 1;
                } else {
                    printf("[*] List command: %s\n", optarg);
                    if (strcmp(optarg, "custom") == 0)
                        list_custom();
                    return 1;
                }
                break;

            case '?':
                return 1;
                break;
        }
    } 

    return 1;
}

#include <string.h>

#include "builtin.h"
#include "jeep.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

int print_usage()
{
    const char usage [] = {
        "jeep \t command [-d]evice -info\n"
        "jeep \t command [-l]ist -custom]\n"
        "jeep \t --about --version\n"
    };

    fprintf(stderr, "%s", usage);

    return -1;
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
               {0, 0, 0, 0}       };

    get_art();

    if (argv[1]) {
        char *cmd = argv[1];

        if (strcmp(cmd, "--about") == 0) {
            printf("jeep\t - penetration testing framework for vehicle systems\n"); 
            exit(0);
        }

        if (strcmp(cmd, "--version") == 0) {
            exit(get_version());
        }

        init_builtin(); /* Initialize builtin cmds */    
        printf("[*] Getting command module: %s\n", cmd);

        if (search_cmd(cmd) == -1) {
            printf("[x] Command not available in built-in system.\n[*] Searching in custom modules.\n");
        }

    }

    if (argc == 0) {
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
            case 'l':
                if (optarg)
                    printf (", %s!\n", optarg);
                else
                    printf ("!\n", optarg);
                break;
            case '?':
                printf("\nProcess got unknown option.\n"); break;
        }
    } 
    return 0;

    
    //print_usage();

    return 1;
}

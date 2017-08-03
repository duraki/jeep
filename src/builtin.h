#ifndef BUILTIN_H
#define BUILTIN_H

#define _DEBUG 1;

const char *cmd[][3] = {
    {"sniff", "Halis Duraki", "Sniff CAN device for datachange.", 1},
    {"dump", "Halis Duraki", "Dump CAN device data.", 1}
};

struct CMD {
   char  title[50];
   char  author[50];
   char  description[500];
   char  name[100];
   int   builtin;
};  

struct cmd_builtin {
    int     id;                 /* id of cmd */
    struct  CMD  command;       /* command */
};
struct cmd_builtin builtin;

int 
init_builtin()
{
    struct CMD c; 
    int total = sizeof(cmd)/sizeof(cmd[0]);

    printf("\n[*] Total commands: %i\n", total);
    printf("[#]\t\tCMD\t\t AUTHOR\t\t\t DESC\t\t\n");

    for (int i = 0; i < total; i++) 
    {
        printf("%i\t\t\%s\t\t %s\t\t %s\t\n", 
                                            i,
                                            cmd[i][0], 
                                            cmd[i][1], 
                                            cmd[i][2]);
    }

    printf("\n");
}

static int
search_cmd(char c[])
{

    for (int i = 0; i < sizeof(cmd)/sizeof(cmd[0]); i++) {
        if (strcmp(c, cmd[i][0]) == 0) {
            printf("[*] Command available; exec ...\n");
            return i;
        }
    }

    return -1;
}

extern const char jeep_usage_string[];
extern const char jeep_more_info_string[];

extern int is_builtin(const char *s);

/** commands **/
extern int cmd_sniff(int argc, const char **argv, const char *prefix);

#endif

#ifndef JEEP_UTIL
#define JEEP_UTIL

#define LOGFILE     "/tmp/jeep.log"

/**
 * @const LOG 
 */
const char *LOG   = "LOG.JEEP";

/**
 * Logger.
 */
int
say(char *module, char *message)
{
    FILE *f;
    time_t dt  = time(NULL);

    //printf("%s\n", message);

    f = fopen(LOGFILE, "a+"); 
    if (f == NULL) { return; }

    if (module[0] == '\0') {
        strcpy(*module, "none"); 
    }

    char *dtb = asctime(localtime(&dt));
    char *logtime[50];
    dtb[strlen(dtb)-1] = 0;

    fprintf(f, "[%s] %s.%s, %s\n", dtb, LOG, module, message);

    fclose(f);
}

#endif

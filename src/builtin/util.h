#define LOGFILE "/tmp/jeep.log"

/**
 * Log data to logfile.
 *
 * message: A log message.
 */
int
flog(char *message)
{
    FILE *f;
    f = fopen(LOGFILE, "a+"); 
    if (f == NULL) { /* Something is wrong   */}

    fprintf(f, "%s\n", message);
    fclose(f);
}

#ifndef BUILTIN_H
#define BUILTIN_H

extern const char jeep_usage_string[];
extern const char jeep_more_info_string[];

extern int is_builtin(const char *s);

/** commands **/
extern int cmd_sniff(int argc, const char **argv, const char *prefix);

#endif
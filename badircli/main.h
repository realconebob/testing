#ifndef _MAIN_H_102820231444
#define _MAIN_H_102820231444

#define PINGREGEX "^(PING :)"
#define REGEX_ERRSTR_LEN 512

#include "irc.h"

int pingrelay(int sfd, struct ircnames name);
int createRegexPattern(regex_t *pattern, const char *PATTERN, int flags);

#endif
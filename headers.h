#ifndef HEADERS_H_
#define HEADERS_H_

// #define scandir(), alphasort(): _BSD_SOURCE || _SVID_SOURCE
#define _GNU_SOURCE

#include <stdio.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <errno.h>
#include <pwd.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <grp.h>
#include <signal.h>
#include <termios.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netdb.h>

#include "prompt.h"
#include "warp.h"
#include "structs.h"
#include "execute.h"
#include "Queue.h"
#include "peek.h"
#include "proclore.h"
#include "seek.h"
#include "pastevents.h"
#include "cmdpastevents.h"
#include "ping.h"
#include "neonate.h"
#include "iMan.h"
#include "activities.h"

extern int current_fg_pid;

#endif
#ifndef __PEEK_H
#define __PEEK_H
#include "structs.h"

#include <dirent.h>
#include <sys/types.h>

void peek(char* address,int lflag,int aflag,Directories* directory);

#endif
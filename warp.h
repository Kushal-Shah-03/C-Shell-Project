#ifndef __WARP_H
#define __WARP_H
#include "structs.h"
// void warp(char* address,char** home_dir,char** presentWD,char** oldWD,int* is_pwd_malloc,int* is_old_wd_malloc);
// void warp(char* address,char** home_dir,char** oldWD);
void warp(char* address,Directories* directory);

#endif
#ifndef __SEEK_H
#define __SEEK_H

void seek(char* input,Directories* directory,int eflag);
void recursedir(char* dirstart,char*curdir,int fflag,int eflag,int dflag,int* count,char* target,char** store,int print);

#endif
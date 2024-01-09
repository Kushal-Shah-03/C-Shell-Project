#ifndef __STRUCTS_H
#define __STRUCTS_H

typedef struct Directories{
    // char* pwd;
    char** owd;
    char** home;
}Directories;

struct t_cmd{
    char* cmd;
    int time;
};

#endif
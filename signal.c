#include "headers.h"

void sig_handler_INT(int signo)
{
    if (current_fg_pid!=-1&&current_fg_pid!=0)
    kill(current_fg_pid,SIGINT);
    // printf("%d\n",current_fg_pid);
//   if (signo == SIGINT)
//     printf("received SIGINT\n");
    // fflush(stdout);
}

void sig_handler_STP(int signo)
{
    // printf("ctr z");
    // fflush(stdout);
    if(current_fg_pid!=-1&&current_fg_pid!=0)
    kill(current_fg_pid,SIGTSTP);
    // printf("%d\n",current_fg_pid);
    // fflush(stdout);
    // if (signo == SIGTSTP)
        // printf("received SIGTSTP");
        // fflush(stdout);
}
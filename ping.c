#include "headers.h"

void ping(char* input)
{
    if (input[0]=='\0')
    {
        printf("\e[0;31mInvalid sytanx for ping\033[0m\n");
        return;
    }
    char* second=NULL;
    for (int i=0;i<strlen(input);i++)
    {
        if (input[i]==' ')
        {
            input[i]='\0';
            second=&input[i+1];
        }
    }
    if (second==NULL)
    {
        printf("\e[0;31mInvalid sytanx for ping\033[0m\n");
        // printf("Invalid sytanx for ping\n");
        return;
    }
    else if (second[0]=='\0')
    {
        // printf("Invalid syntax for ping\n");
        printf("\e[0;31mInvalid sytanx for ping\033[0m\n");
        return;
    }
    int pid = atoi(input);
    int sig = atoi(second);
    if (pid<0)
    {
        // printf("No such process found\n");
        printf("\e[0;31mNo such process found\033[0m\n");
        return;
    }
    int check=kill(pid,sig%32);
    if (check==-1)
    {
        if (errno==ESRCH)
        printf("\e[0;31mNo such process found\033[0m\n");
        else
        {
            printf("\e[0;31mInvalid Signal or No permissions\033[0m\n");
            // printf("Invalid Signal or No permissions");
        }
        return;
    }
    printf("Sent signal %d to process with pid %d\n",sig%32,pid);
    
}
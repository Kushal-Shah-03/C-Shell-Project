#include "headers.h"


char* getVmem(char* path,char* line)
{
    FILE* f1 = fopen(path, "r");
    if (!f1)
    {
        printf("Invalid PID\n");
        return NULL;
    }
    char* result;
    while (fgets(line, 256, f1) != NULL)
    {
        if (strncmp(line, "VmSize:", 7) == 0){
            result = &line[7];
            int i=0;
            while (result[i]==' '|| result[i]=='\t')
            {
                i++;
            }
            result=result+i;
            break;
        }
    }
    fclose(f1);
    return result;
}

char* getState(char* path,char* line)
{
    FILE* f1 = fopen(path, "r");
    if (!f1)
    {
        printf("No permissions\n");
        return NULL;
    }
    char* result;
    while (fgets(line, 256, f1) != NULL)
    {
        if (strncmp(line, "State:", 6) == 0){
            result = &line[6];
            int i=0;
            while (result[i]==' '|| result[i]=='\t')
            {
                i++;
            }
            result=result+i;
            result[1]='\0';
            break;
        }
    }
    fclose(f1);
    return result;
}

void proclore(char* input)
{
    // printf("%s\n",input);
    int currentpid=getpid();
    char path[128];
    char vmpath[32];
    // char statuspath[32];
    int pid=0;
    if (strlen(input)==0)
    {
        pid = getpid();
        sprintf(path, "/proc/%d/exe", pid);
        sprintf(vmpath,"/proc/%d/status",pid);
    }   
    else
    {
        if(input[strlen(input)-1]==' '||input[strlen(input)-1]=='\t'||input[strlen(input)-1]=='\n')
        input[strlen(input)-1]='\0';
        if (input[0]=='-')
        {
            printf("Invalid pid\n");
            return;
        }
        for (int i=0;i<strlen(input);i++)
        {
            pid=pid*10+(input[i]-48);
        }
        sprintf(path, "/proc/%s/exe", input);
        sprintf(vmpath,"/proc/%s/status",input);
        // printf("%s",path);
    }

    int foregroundid = tcgetpgrp(STDIN_FILENO);
    int grpid = getpgid(pid);
    char* plus="\0";
    if (foregroundid==grpid)
    {
        plus="+";
    }

    char* buffer=malloc(sizeof(char)*4096);
    char* line=malloc(sizeof(char)*256);
    char status[3];
    status[0]='\0';
    printf("pid : %d\n",pid);
    char procPath[4096];
    sprintf(procPath, "/proc/%d/stat", pid);
    FILE* procFile = fopen(procPath, "r");
    if (!procFile)
    {
        printf("Invalid Pid\n");
        // return NULL;
        return;
    }
    (fgets(buffer, 4096, procFile));
    int wordCount = 0;
    int runningGroup;
    char processStatus[4];
    char *token = strtok(buffer, " ");
    while (token != NULL)
    {
        if (wordCount == 7)
        {
            runningGroup = atoi(token);
        }
        wordCount++;
        token = strtok(NULL, " ");
    }
    if (runningGroup == pid)
    {
        processStatus[1] = '+';
        processStatus[2] = '\0';
    }
    // State:	S (sleeping)
    // isse if i can find the state to bas + i can add by iterating thru the queue ig idk yet
    buffer=malloc(sizeof(char)*4096);
    char* idek=getState(vmpath,line);
    if (idek==NULL)
    return;
    else
    strcat(idek,plus);
    processStatus[0]=idek[0];
    printf("process status : %s\n",processStatus);
    // maybe don't use getpdgid idk
    printf("Process Group : %d\n",getpgid(pid));
    printf("Virtual memory : %s",getVmem(vmpath,line));
    free(line);
    int bytes=readlink(path,buffer,4096);
    if (bytes==-1)
    {
        printf("\033[0;31mExecutable path can't be traced\033[0m\n");
        // printf("Invalid Process ID\n");
        return;
    }
    buffer[bytes]='\0';
    printf("executable path : %s\n",buffer);
    free(buffer);
}
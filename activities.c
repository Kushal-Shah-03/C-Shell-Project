#include "headers.h"

char* getState(char* path,char* line);

int cmp (const void * a, const void * b) {
    const Node* c=a;
    const Node* d=b;
    char c2[10];
    char d2[10];
    sprintf(c2,"%d",c->pid);
    // printf("a%sa\n",c2);
    sprintf(d2,"%d",d->pid);
    // printf("a%sa\n",d2);
    fflush(stdout);
    return strcmp(c2,d2);
}

void activities(Que** Q,Directories* directoryptr) 
{
    int bgstatus;
    // Node* list[i];
    Node* bgproc;
    int bgflag;
    Node list[(*Q)->count];
    for (int i=0;i<(*Q)->count;i++)
    {
        bgproc=Deque(*Q);
        list[i]=*bgproc;
        Queue(*Q,bgproc->pid,bgproc->cmd);
    }
    qsort(list,(*Q)->count,sizeof(Node),cmp);
    for (int i=0;i<(*Q)->count;i++)
    {
        // list[i]=Top(Q);
        // printf("%s",list[i]->cmd);
        // printf("%d",list[i]->pid);
        // else
        // char buffer[4096];
        char vmpath[4096];
        sprintf(vmpath,"/proc/%d/status",list[i].pid);
        // sprintf(procPath, "/proc/%d/stat", list[i]->pid);
        // FILE* procFile = fopen(procPath, "r");
        // (fgets(buffer, 4096, procFile)); 
        char* line=malloc(sizeof(char)*256);    
        char* idek=getState(vmpath,line);
        if (idek==NULL)
        {
            // Queue(*Q,list[i]->pid,list[i]->cmd);
            continue;
        }
        else
        {
            if (idek[0]=='T')
            {
                printf("%d : %s - Stopped\n",list[i].pid,list[i].cmd);
            }
            else
            {
                printf("%d : %s - Running\n",list[i].pid,list[i].cmd);
            }
            // Queue(*Q,list[i]->pid,list[i]->cmd);
        }
        // free (list[i]);
    }
}              
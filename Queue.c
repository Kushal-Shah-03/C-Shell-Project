#include "Queue.h"
#include <stdlib.h>
#include <stdio.h>
// void Enque(Queue* Q,int pid, char* cmd)
// {
//     if(Q->front<255)
//     {
//         Q->front++;
//     }
//     Q->Processes[Q->front].pid=pid;
//     Q->Processes[Q->front].cmd=cmd;
//     return;
// }

// BG_proc* Top (Queue* Q){
//     return &(Q->Processes[Q->front]);
// }
// void Deque(Queue* Q)
// {
//     if(Q->front>=0)
//     Q->front--;
//     else
//     {
//         printf("Queue empty\n");
//         return;
//     }
// }
// Queue* Initque ()
// {
//     Queue* Q=malloc(sizeof(Queue));
//     Q->front=0;
//     Q->rear=-1;
//     return Q;
// }

// int IsEmptyQue(Queue* Q)
// {
//     if (Q->rear==-1)
//     {
//         return 1;
//     }
//     else
//     {
//         return 0;
//     }
// }



void Queue (Que* Q,int pid,char* cmd)
{
    Node* T = malloc (sizeof(Node));
    T->Next=NULL;
    T->pid=pid;
    T->cmd=cmd;
    Q->count++;
    if (T==NULL)
    return;
    if (IsEmpty(Q)==0)
    {
    Q->Tail->Next=T;
    Q->Tail=T;
    }
    else
    {
        Q->Head=T;
        Q->Tail=Q->Head;
        Q->Head->Next=NULL;
    }
}
Node* Deque (Que* Q)
{
    Node* temp=Q->Head;
    Q->Head=Q->Head->Next;
    Q->count--;
    return temp;
}
int IsEmpty(Que* Q)
{
    if (Q->Tail==NULL||Q->Head==NULL)
    {
        return 1;
    }
    else 
    return 0;
}
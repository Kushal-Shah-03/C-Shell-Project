#ifndef __QUEUE_H
#define __QUEUE_H

typedef struct BG_proc{
    int pid;
    char* cmd;
    struct BG_proc* Next;
}BG_proc;

typedef BG_proc Node;
// typedef struct Queue
// {
//     int front;
//     int rear;
//     BG_proc Processes[256];
// }Queue;

// void Enque(Queue* Q,int pid, char* cmd);
// BG_proc* Top (Queue* Q);
// void Deque(Queue* Q);
// Queue* Initque ();
// int IsEmptyQue(Queue* Q);

// typedef stTree Node;
typedef struct Queue{
    Node* Head;
    Node* Tail;
    int count;
}Que;

int IsEmpty(Que* Q);
Node* Deque (Que* Q);
void Queue (Que* Q,int pid, char* cmd);



#endif
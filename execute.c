#include "headers.h"

char* getState(char* path,char* line);

void sig_handler_INT(int signo);
void sig_handler_STP(int signo);

int execute (char* input,int flag,Directories* directoryptr,Que** Q)
{
    int execpid=-2;
    int statusofexec;
    int stout=dup(STDOUT_FILENO);
    // char f1path[256];
    // strcpy(f1path,*directoryptr->home);
    // strcat(f1path,"/a.txt"); 
    // int f1=open(f1path, O_RDWR|O_CREAT|O_TRUNC, 0600);
    // // dup2(f1,STDOUT_FILENO);
    // int f2=open(f1path, O_RDWR|O_CREAT|O_APPEND, 0600);
    int stin=dup(STDIN_FILENO);
    // f1=open("a.txt", O_RDWR|O_CREAT|O_APPEND, 0600);
    // dup2(f2,STDIN_FILENO);
    // printf("Hi\n");
    // goes in a.txt
    char* savepipe;
    int inputfile;
    int outputfile;
    int piped_count=0;
    for (int count=0;count<strlen(input);count++)
    {
        if (input[count]=='|')
        piped_count++;
    }
    if (piped_count>=1)
    {
        char* piped = strtok_r(input,"|",&savepipe);
        // counts of how many pipes
        while(piped_count!=0)
        {
            piped_count--;
            int pd[2];
            pipe(pd);
            if (piped[strlen(piped)-1]==' '||piped[strlen(piped)-1]=='\n')
            piped[strlen(piped)-1]='\0';
            if (piped[0]==' '||piped[0]=='\n')
            piped=&piped[1];
            if (fork()==0)
            {
                dup2(pd[1], 1);
                execute(piped,0,directoryptr,Q);
                abort();
            }
            dup2(pd[0], 0);
            close(pd[1]);
            piped=strtok_r(NULL,"|",&savepipe);
        }
        if (piped[strlen(piped)-1]==' '||piped[strlen(piped)-1]=='\n')
            piped[strlen(piped)-1]='\0';
        execute(piped,0,directoryptr,Q);
        fflush(stdout);
        dup2(stin,STDIN_FILENO);
        dup2(stout,STDOUT_FILENO);
        return execpid;
    }
    if (piped_count==0)
    {
        // input=piped; 
        // int f2=open(f1path, O_RDWR|O_CREAT|O_APPEND, 0600);
        stout=dup(STDOUT_FILENO);
        stin=dup(STDIN_FILENO);
        // int f1=open(f1path, O_RDWR|O_CREAT|O_TRUNC, 0600);
        // dup2(f1,STDOUT_FILENO);
        // dup2(f2,STDIN_FILENO);
        // input=piped;
        if (input[0]==' '||input[0]=='\n'||input[0]=='\t')
        {
            input=&input[1];
        }
        if (input[strlen(input)-1]==' '||input[strlen(input)-1]=='\n')
        {
            input[strlen(input)-1]='\0';
        }
        int lesserarrow=0;
        int greaterarrow=0;
        char* filepath;
        int len=strlen(input);
        for (int counting=0;counting<len;counting++)
        {
            if (input[counting]=='<')
            {
                // printf("a<a");
                input[counting]='\0';
                if (input[counting]==' ')
                {
                    input[counting-1]='\0';
                }
                lesserarrow++;
                if (input[counting+1]==' ')
                {
                    counting++;
                }
                filepath=&input[counting+1];
                for (int countering=0;countering<strlen(filepath);countering++)
                {
                    if (filepath[countering]==' '||filepath[countering]=='\n')
                    filepath[countering]='\0';
                }
                if (filepath[strlen(filepath)-1]==' ')
                filepath[strlen(filepath)-1]='\0';
                // printf("a %s a",filepath);
                if (filepath[0]=='~')
                {
                    char temp[256];
                    strcpy(temp,filepath);
                    strcpy(filepath,*directoryptr->home);
                    strcat(filepath,temp);
                }
                if (lesserarrow==1)
                inputfile=open(filepath, O_RDONLY, 0600);
                else
                return -2;
                if (inputfile==-1)
                {
                    printf("No such input file found!");
                    return -2;
                }
                dup2(inputfile,STDIN_FILENO);
                // take filename after arrow ig
            }
            if (input[counting]=='>')
            {
                // printf("a>a");
                while (input[counting]=='>')
                {
                    input[counting]='\0';
                    greaterarrow++;
                    counting++;
                }
                if (input[counting]==' ')
                counting++;
                filepath=&input[counting];
                for (int countering;countering<strlen(filepath);countering++)
                {
                    if (filepath[countering]==' '||filepath[countering]=='\n')
                    filepath[countering]='\0';
                }
                if (filepath[strlen(filepath)-1]==' ')
                filepath[strlen(filepath)-1]='\0';
                // printf("a %s a",filepath);
                if (filepath[0]=='~')
                {
                    char temp[256];
                    strcpy(temp,filepath);
                    strcpy(filepath,*directoryptr->home);
                    strcat(filepath,temp);
                }
                if (greaterarrow==1)
                outputfile=open(filepath, O_WRONLY|O_CREAT|O_TRUNC, 0644);
                if (greaterarrow==2)
                outputfile=open(filepath, O_WRONLY|O_CREAT|O_APPEND, 0644);
                if (outputfile==-1)
                {
                    printf("No such input file found!");
                    return -2;
                }
                dup2(outputfile,STDOUT_FILENO);
                // take filename below arrow ig
            }
        }
    // printf("ab %s ab\n",input);
    if (strncmp(input,"warp",4)==0&&(strlen(input)>=4||(strlen(input)>=5&&(input[4]==' '||input[4]=='\n'||input[4]=='\0'))))
    {
        if (strlen(input)==4)
        {
            warp("~",directoryptr);
            return execpid;
        }
        char* input2=&input[5];
        if (strlen(input2)==0)
        warp("~",directoryptr);
        char* arg_of_warp;
        while (1)
        {
            arg_of_warp=strtok(input2," ");
            // printf("%s",arg_of_warp);
            if(arg_of_warp==NULL)
            break;
            warp(arg_of_warp,directoryptr);
            input2=NULL;
        }
        // printf("hi\n");
    }
    else if (strncmp(input,"peek",4)==0)
    {
        char* input2=&input[5];
        // printf("a%sa\n",input2);
        // chdir(input2);
        // chdir("..");
        // printf("%s\n",input2);
        peek(input2,0,0,directoryptr);
    }
    else if (strncmp(input,"proclore",8)==0)
    {
        char* input2=&input[9];
        proclore(input2);
    }
    else if (strncmp(input,"seek",4)==0)
    {
        char*input2=&input[5];
        seek(input2,directoryptr,0);
    }
    else if (strncmp(input,"pastevents purge",16)==0)
    {
        if (input[strlen(input)-1]==' '||input[strlen(input)-1]=='\t'||input[strlen(input)-1]=='\n')
        input[strlen(input)-1]='\0';
        if (strncmp(input,"pastevents purge",strlen(input))==0)
        exec_pasteventspurge(directoryptr);
        else
        printf("\033[0;31mInvalid Command\033[0m\n");
    }
    else if (strncmp(input,"pastevents",10)==0)
    {
        if (input[strlen(input)-1]==' '||input[strlen(input)-1]=='\t'||input[strlen(input)-1]=='\n')
        input[strlen(input)-1]='\0';
        if (strncmp(input,"pastevents",strlen(input))==0)
        exec_pastevents(directoryptr);
        else
        {
            printf("\033[0;31mInvalid Command\033[0m\n");
        }
    }
    else if (strncmp(input,"ping",4)==0)
    {
        if (input[strlen(input)-1]==' '||input[strlen(input)-1]=='\t'||input[strlen(input)-1]=='\n')
        input[strlen(input)-1]='\0';
        if (strlen(input)<6)
        {
            printf("\033[0;31mInvalid syntax for ping\033[0m\n");
            // printf("Invalid syntax for ping\n");
            return -2;
        }
        input=&input[5];
        ping(input);
    }
    else if (strncmp(input,"activities",10)==0)
    {
        if (input[10]=='\0'||((input[10]==' '||input[10]=='\t'||input[10]=='\n')&&input[11]=='\0'))
        {
            if (IsEmpty(*Q)!=1)
            {
                // printf("%d",Q->index);
                activities(Q,directoryptr);
            }
        }
        else
        {
            printf("\033[0;31mInvalid syntax for ping\033[0m\n");
            // printf("Invalid syntax for activities\n");
            return -2;
        }
    }
    else if (strncmp(input,"fg",2)==0)
    {
        input=&input[2];
        // if (input[0]=='\0'||input[0]=='\n')
        if (input[0]==' ')
        {    
            input=&input[1];
            int pid=atoi(input);
            char vmpath[4096];
            sprintf(vmpath,"/proc/%d/status",pid);
            // sprintf(procPath, "/proc/%d/stat", bgproc->pid);
            // FILE* procFile = fopen(procPath, "r");
            // (fgets(buffer, 4096, procFile)); 
            char* line=malloc(sizeof(char)*256);    
            char* idek=getState(vmpath,line);
            if (idek==NULL)
            {
                printf("\033[0;31mNo such process found\033[0m\n");
                // printf("No such process found\n");
                return -2;
            }
            if (IsEmpty(*Q)!=1)
            {
                // printf("%d",Q->index);
                int bgstatus;
                Node* bgproc;
                int bgflag;
                for (int i=0;i<(*Q)->count;i++)
                {
                    // bgproc=Top(Q);
                    // printf("%s",bgproc->cmd);
                    bgproc=Deque(*Q);
                    if (bgproc->pid!=pid)
                    {
                        Queue(*Q,bgproc->pid,bgproc->cmd);
                    }
                }
            }
            if (idek[0]=='T')
            {
                // printf("a%da\n",pid);
                current_fg_pid=pid;
                char* wth=malloc(sizeof(char)*100);
                sprintf(wth,"fg %d",current_fg_pid);
                // printf("%d\n",pid);
                signal(SIGINT,SIG_IGN);
                signal(SIGTSTP,SIG_IGN);
                signal(SIGTTIN, SIG_IGN);
                signal(SIGTTOU, SIG_IGN);
                tcsetpgrp(STDIN_FILENO,pid);
                kill(pid,SIGCONT);
                setpgid(pid,pid);
                waitpid(pid,&statusofexec,WUNTRACED);
                if(WIFEXITED(statusofexec)==1)
                {
                    // printf("Hi\n");
                    // fflush(stdout);
                    // perror("Error");
                }
                else
                {
                    Queue(*Q,current_fg_pid,wth);
                }
                tcsetpgrp(STDIN_FILENO,getpgrp());
                signal(SIGTTOU, SIG_DFL);
                signal(SIGTTIN, SIG_DFL);
                signal(SIGINT,SIG_DFL);
                signal(SIGTSTP,SIG_DFL);
            }
            else
            {
                // printf("a%da\n",pid);
                signal(SIGTTIN, SIG_IGN);
                signal(SIGTTOU, SIG_IGN);
                tcsetpgrp(STDIN_FILENO,pid);
                setpgid(pid,pid);
                // printf("Heck");
                current_fg_pid=pid;
                char* wth=malloc(sizeof(char)*100);
                sprintf(wth,"fg %d",current_fg_pid);
                // printf("a%da\n",pid);
                signal(SIGINT,SIG_IGN);
                signal(SIGTSTP,SIG_IGN);
                waitpid(pid,&statusofexec,WUNTRACED);
                if(WIFEXITED(statusofexec)==1)
                {
                    // printf("Hi\n");
                    // fflush(stdout);
                    // perror("Error");
                }
                else
                {
                    Queue(*Q,current_fg_pid,wth);
                }
                tcsetpgrp(STDIN_FILENO,getpgrp());
                signal(SIGTTOU, SIG_DFL);
                signal(SIGTTIN, SIG_DFL);
                signal(SIGINT,SIG_DFL);
                signal(SIGTSTP,SIG_DFL);
            }
            // printf("%d\n",pid);
            
        }
        else
        {
            printf("Invalid Syntax for fg\n");
        }
    }
    else if (strncmp(input,"bg",2)==0)
    {
        input=&input[2];
        if (input[0]==' ')
        {
            input=&input[1];
            int pid=atoi(input);
            char vmpath[4096];
            sprintf(vmpath,"/proc/%d/status",pid);
            // sprintf(procPath, "/proc/%d/stat", bgproc->pid);
            // FILE* procFile = fopen(procPath, "r");
            // (fgets(buffer, 4096, procFile)); 
            char* line=malloc(sizeof(char)*256);    
            char* idek=getState(vmpath,line);
            if (idek==NULL)
            {
                printf("No such process found\n");
            }
            else if (idek[0]=='T')
            {
                kill(pid,SIGCONT);
            }
        }
        else
        {
            printf("Invalid Syntax for bg\n");
            // return -2;
        }
    }
    else if (strncmp(input,"neonate",7)==0)
    {
        input=&input[7];
        // if (input[0]=='\0')
        if (strlen(input)<5)
        {
            printf("Invalid syntax for neonate\n");
            return -2;
        }
        if (input[0]==' '&&input[1]=='-'&&input[2]=='n'&&input[3]==' ')
        {
            input=&input[4];
            if (input[0]=='\0')
            {
                printf("Invalid syntax for neonate\n");
                return -2;
            }
            int secs=atoi(input);
            if (secs<0)
            {
                printf("Invalid value for neonate\n");
            }
            else
            // printf("%d\n",secs);
            neonate(secs);
        }
        return -2;

    }
    else if (strncmp(input,"iMan",4)==0)
    {
        if (input[4]==' ')
        {
            input=&input[5];
            if (input[0]!='\0')
            iMan(input);
        }
        else
        {
            printf("Invalid Syntax for iMan");
        }
    }
    else
    {
        // if (getpid()!=current_fg_pid&&kill(current_fg_pid,0)==0)
        // Queue(*Q,current_fg_pid,input);
        char* arguments[]={"bash","-c",input,NULL};
        execpid=fork();
        if (execpid==-1)
        {
            perror("Unsuccesful fork");
            return -2;
        }
        else if (execpid==0)
        {
            // if (signal(SIGINT, sig_handler_INT) == SIG_ERR)
            // printf("\ncan't catch SIGINT\n");
            // if (signal(SIGTSTP,sig_handler_STP)==SIG_ERR)
            // printf("can't catch SIGTSTP\n");
            // if (fla)
            setpgid(0,0);
            // if execvp gives wrong command to bash print hoga chalega or do i need to change
            if (execvp("bash",arguments)==-1)
            {
                // printf("Hi");
                perror("Error in Executing");
                exit(0);
            }
        }
        else
        {
            current_fg_pid=execpid;
            if (flag==0)
            {
                signal(SIGTTIN, SIG_IGN);
                signal(SIGTTOU, SIG_IGN);
                tcsetpgrp(STDIN_FILENO,execpid);
                setpgid(execpid,execpid);
                    // printf("Heck");
                    current_fg_pid=execpid;
                    // char* wth=malloc(sizeof(char)*100);
                    // sprintf(wth,"fg %d",current_fg_pid);
                    // printf("a%da\n",pid);
                    signal(SIGINT,SIG_IGN);
                    signal(SIGTSTP,SIG_IGN);
                    waitpid(execpid,&statusofexec,WUNTRACED);
                    if(WIFEXITED(statusofexec)==1)
                    {
                        // printf("Hi\n");
                        // fflush(stdout);
                        // perror("Error");
                    }
                    else
                    {
                        // Queue(*Q,current_fg_pid,input);
                    }
                    tcsetpgrp(STDIN_FILENO,getpgrp());
                    signal(SIGTTOU, SIG_DFL);
                    signal(SIGTTIN, SIG_DFL);
                    signal(SIGINT,SIG_DFL);
                    signal(SIGTSTP,SIG_DFL);
            }
            // if (flag==1)
            // waitpid(execpid,&statusofexec,WUNTRACED);
            if(WIFEXITED(statusofexec)==0)
            {
                // perror("Error");
            }
        }
    }
    if (lesserarrow==1)
        {
            close(inputfile);
            dup2(stin,STDIN_FILENO);
        }
        if (greaterarrow>0)
        {
            close(outputfile);
            dup2(stout,STDOUT_FILENO);
        }
        // piped=strtok_r(NULL,"|",&savepipe);
        // fflush(stdout);
        // fflush(stdin);
        // close(f1);
        // dup2(stout,STDOUT_FILENO);
        // close(f2);
        // dup2(stin,STDIN_FILENO);
    }
    dup2(stin,STDIN_FILENO);
    dup2(stout,STDOUT_FILENO);
    return execpid;
}

#include "headers.h"

void sig_handler_INT(int signo);
void sig_handler_STP(int signo);

int current_fg_pid;

int main()
{
  if (signal(SIGINT, sig_handler_INT) == SIG_ERR)
  printf("\ncan't catch SIGINT\n");
  if (signal(SIGTSTP,sig_handler_STP)==SIG_ERR)
  printf("can't catch SIGTSTP\n");
  // A long long wait so that we can easily issue a signal to this 
  // process
    // storing the directory the shell started in to store it as home directory
    struct timeval start, end;
    long t_micros;
    float t_seconds;
    int i_t_seconds;
    int t_show=0;
    // double t_proc; 
    Directories directory;
    Que* Q=(Que*)malloc(sizeof(Que));
    Q->Head=NULL;
    Q->Tail=NULL;
    Q->count=0;
    char* home_dir=NULL;
    home_dir=getcwd(home_dir,0);
    char*presentWD=home_dir;
    char* oldWD=NULL;
    int is_pwd_malloc=0;
    int is_old_wd_malloc=0;
    int home_len=strlen(home_dir);
    directory.home=&home_dir;
    directory.owd=&oldWD;
    BG_proc* bgproc;
    int bgflag;
    int bgstatus;
    struct t_cmd prev_cmd;
    prev_cmd.cmd=malloc(sizeof(char)*4096);
    int prompt_flag=0;
    int ampernospace;
    // write for handling errors here

    // Keep accepting commands
    while (1)
    {
        if (signal(SIGINT, sig_handler_INT) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");
        if (signal(SIGTSTP,sig_handler_STP)==SIG_ERR)
        printf("can't catch SIGTSTP\n");
        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt(home_dir,home_len,t_show,&prev_cmd);
        t_show=0;
        char* input=malloc(sizeof(char)*4096);
        // fgets(input, 4096, stdin);
        //checks for ctrl D
        if (!fgets(input, 4096, stdin))
        {
            // printf("Bhai sahab");
            fflush(stdout);
            kill(-1*getpid(),SIGKILL);
            // exit(0);
        }
        char* input2=input;
        input=malloc(sizeof(char)*(4096));
        int j=0;
        for (int i=0;i<strlen(input2);i++)
        {
            if (input2[i]=='&')
            {
                input[j]='&';
                input[j+1]=' ';
                j++;
            }
            else
            {
                input[j]=input2[i];
            }
            j++;
        }
        input[j]='\0';
        // printf("%s",input);
        if (IsEmpty(Q)!=1)
        {
            // printf("%d",Q->index);
            for (int i=0;i<Q->count;i++)
            {
                // bgproc=Top(Q);
                // printf("%s",bgproc->cmd);
                bgproc=Deque(Q);
                // printf("%d",bgproc->pid);
                bgflag=waitpid(bgproc->pid,&bgstatus,WNOHANG);
                if (bgflag)
                {
                    if (WIFEXITED(bgstatus))
                    printf("Process with pid: %d, terminated successfully, command: %s\n",bgproc->pid,bgproc->cmd);
                    else
                    printf("Process with pid: %d, terminated abnormarlly, command: %s\n",bgproc->pid,bgproc->cmd);
                    free (bgproc->cmd);
                }
                else
                Queue(Q,bgproc->pid,bgproc->cmd);
                free (bgproc);
            }
        }
        char* parsed_input=malloc(sizeof(char)*4096);
        parsed_input[0]='\0';
        char* savptrpipe;
        char* temp=strtok_r(input,"|",&savptrpipe);
        while (temp!=NULL)
        {
            strcat(parsed_input,pastevents(temp,&directory));
            temp=strtok_r(NULL,"|",&savptrpipe);
            if (temp!=NULL)
            strcat(parsed_input," | ");
            
        }
        // parsed_input=parsedfully(input);
        // printf("%s",parsed_input);
        input=parsed_input;
        char* laterparseinput=malloc(sizeof(char)*1024);
        strcpy(laterparseinput,parsed_input);
        char* tokenized_input=calloc(4096,sizeof(char));
        char* saveptr_semicol;
        char* saveptr_amper;
        char* saveptr_space;
        char* token_semicol;
        char* token_amper;
        char* token_space;
        // input=parsed_input;
        // printf("%s",input);
        // // // // // warp(input,&home_dir,&presentWD,&oldWD,&is_pwd_malloc,&is_old_wd_malloc);
        // warp(input,&home_dir,&oldWD);
        while (1)
        {
            token_semicol= strtok_r(input,";",&saveptr_semicol);
            if(token_semicol==NULL)
            {
                break;
            }
            else
            {
                ampernospace=0;
                if (token_semicol[strlen(token_semicol)-1]=='&')
                ampernospace=1;
                while (1)
                {
                    // printf("%s\n",tokenized_input);
                    token_amper=strtok_r(token_semicol,"&",&saveptr_amper);
                    if (token_amper==NULL&&ampernospace!=1)
                    {
                        if(tokenized_input[0]!='\0')
                        {
                            // printf("Hi %s",tokenized_input);
                            //run if fg;
                            gettimeofday(&start, NULL);
                            // int stlen=strlen(tokenized_input);
                            char* idek=(char*)malloc(sizeof(char)*1024);
                            strcpy(idek,tokenized_input);
                            // printf("%s",tokenized_input);
                            int pid_temp=execute(tokenized_input,0,&directory,&Q);
                            // if (getpid()!=pid_temp&&kill(pid_temp,0)==0)
                            // Queue(Q,pid_temp,idek);
                            current_fg_pid=pid_temp;
                            if (pid_temp==-2)
                            {
                                // lite;
                            }
                            else
                            {
                                if (kill(pid_temp,0)==0)
                                {
                                    char* temp_str=malloc(sizeof(char)*1024);
                                    strcpy(temp_str,tokenized_input);
                                    Queue(Q,pid_temp,temp_str);
                                    printf("%d\n",pid_temp);
                                }
                            }
                            // tokenized_input=" ";
                            gettimeofday(&end, NULL);
                            t_micros = (((end.tv_sec - start.tv_sec) * 1000000) + end.tv_usec) - (start.tv_usec);
                            t_seconds=t_micros/1000000.0;
                            // printf("%f\n",t_micros/1000000.0);
                            if (t_seconds>2+1e-6)
                            {
                                t_show=1;
                                int i_t_seconds=(int)(t_seconds+0.5);
                                // printf("%d",i_t_seconds);
                                strcpy(prev_cmd.cmd,tokenized_input);
                                prev_cmd.time=i_t_seconds;
                            }
                        }
                        tokenized_input[0]='\0';
                        break;
                    }
                    else
                    {
                        if(tokenized_input[0]!='\0')
                        {
                            int bgpid;
                            // run in bg;
                            // printf("Bye %s",tokenized_input);
                            // int bgpid=fork();
                            // if (bgpid==-1)
                            // {
                            //     perror("Unsuccesful fork for bg process");
                            // }
                            // if (bgpid==0)
                            // {
                                // setpgid(0,0);
                                // perror("Hi");
                            // char idek[512];
                            // strcpy(idek,tokenized_input);
                            int temp_pid =execute(tokenized_input,1,&directory,&Q);
                            // current_fg_pid=temp_pid;
                                // tokenized_input=" ";
                                // exit(0);
                            // }
                            // else
                            // {
                                // setpgid(bgpid, getpid());
                                bgpid=current_fg_pid;
                                printf("%d\n",bgpid);
                                char* bg_cmd=malloc(sizeof(char)*1024);
                                strcpy(bg_cmd,tokenized_input);
                                Queue(Q,bgpid,bg_cmd);
                                // tokenized_input[0]='\0'; 
                                // remember to free the string after printing it and dequeing it
                            // }
                            tokenized_input[0]='\0';
                            if (token_amper==NULL)
                            break;
                        }
                        tokenized_input[0]='\0';
                        while(1)
                        {
                            // am not sure if i shud add \n here but ok
                            token_space=strtok_r(token_amper," \t",&saveptr_space);
                            if(token_space==NULL)
                            {
                                break;
                            }
                            else
                            {
                                if(token_space[strlen(token_space)-1]=='\n')
                                {
                                    token_space[strlen(token_space)-1]='\0';
                                }
                                if(strlen(token_space)==0)
                                break;
                                strcat(tokenized_input,token_space);
                                strcat(tokenized_input," ");
                                // printf("%s ab\n",token_space);
                            }
                            token_amper=NULL;
                        }
                    }
                    token_semicol=NULL;
                }

            }
            input=NULL;
        }
        // for (j = 1, str1 = argv[1]; ; j++, str1 = NULL) {
        //        token = strtok_r(str1, argv[2], &saveptr1);
        //        if (token == NULL)
        //            break;
        //        printf("%d: %s\n", j, token);

        //        for (str2 = token; ; str2 = NULL) {
        //            tokenized_after_semicol = strtok_r(str2,';' , &saveptr2);
        //            if (subtoken == NULL)
        //                break;
        //            printf("\t --> %s\n", subtoken);
        //        }
        //    }

        // char* tokenized_input=strtok(input," ");
        // input=NULL;
        // while (tokenized_input!=NULL)
        // {
        //     // printf("%s",tokenized_input);
        //     tokenized_input=strtok(input," ");
        //     input=NULL;
        // }
        // free(original_input);
        // printf("%s",laterparseinput);
        char* addtofile=malloc(sizeof(char)*1024);
        // printf("%s\n",laterparseinput);
        strcpy(addtofile,laterparseinput);
        int flagger=parsedfully(laterparseinput);
        if (flagger)
        {
            writeinfile(addtofile,&directory);
            // printf("%s",addtofile);
        }
        
        // printf("%s\n",addtofile);
    }
    free(home_dir);
    
}

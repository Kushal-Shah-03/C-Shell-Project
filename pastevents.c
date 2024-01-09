#include "headers.h"

int checkforpasteventsexecute(char* tokenized_input)
{
    int i=-1;
    if (strncmp(tokenized_input,"pastevents execute",18)==0)
    {
        if (tokenized_input[19]>='0'&&tokenized_input[19]<='9')
        {
            i=tokenized_input[19]-'0';
            if (tokenized_input[20]>='0'&&tokenized_input[20]<='9')
            {
                if (tokenized_input[21]==' '||tokenized_input[21]=='\0'||tokenized_input[21]=='\n')
                i=10*i+(tokenized_input[20]-'0');
                else
                i=-1;
            }
            else if (tokenized_input[20]==' '||tokenized_input[20]=='\0'||tokenized_input[20]=='\n')
            {
                
            }
            else
            i=-1;
            // printf("%c\n",tokenized_input[19]);
        }
        return i;
    }
    return i;
}

char* pastevents(char* input,Directories* directory)
{
    char* returned_input=(char*)malloc(sizeof(char)*1024);
    returned_input[0]='\0';
    struct timeval start, end;
    long t_micros;
    float t_seconds;
    int i_t_seconds;
    int t_show=0;
    // double t_proc; 
    // Directories directory;
    // char* home_dir=NULL;
    // home_dir=getcwd(home_dir,0);
    // char*presentWD=home_dir;
    // char* oldWD=NULL;
    int is_pwd_malloc=0;
    int is_old_wd_malloc=0;
    // int home_len=strlen(home_dir);
    // directory.home=&home_dir;
    // directory.owd=&oldWD;
    BG_proc* bgproc;
    int bgflag;
    int bgstatus;
    struct t_cmd prev_cmd;
    prev_cmd.cmd=malloc(sizeof(char)*4096);
    int prompt_flag=0;
    int ampernospace;
        char* tokenized_input=calloc(4096,sizeof(char));
        char* saveptr_semicol;
        char* saveptr_amper;
        char* saveptr_space;
        char* token_semicol;
        char* token_amper;
        char* token_space;

        while (1)
        {
            token_semicol= strtok_r(input,";",&saveptr_semicol);
            if(token_semicol==NULL)
            {
                returned_input[strlen(returned_input)-1]='\0';
                break;
            }
            else
            {
                ampernospace=0;
                // if (token_semicol[strlen(token_semicol)-1]==' '||token_semicol[strlen(token_semicol)-1]=='\n')
                // token_semicol[strlen(token_semicol)-1]='\0';
                // if (strlen(token_semicol)>1)
                // {
                if (token_semicol[strlen(token_semicol)-1]=='&')
                    ampernospace=1;
                // }
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
                            if (checkforpasteventsexecute(tokenized_input)==-1)
                            strcat(returned_input,tokenized_input);
                            else
                            {
                                int val=checkforpasteventsexecute(tokenized_input);
                                char* line = getcmdpastevents(val,directory);
                                // printf("%s",line);
                                if(line!=NULL)
                                strcat(returned_input,line);
                                // printf("%d\n",val);
                            }
                            // strcat(returned_input,"&");
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
                            if (checkforpasteventsexecute(tokenized_input)==-1)
                            {
                                strcat(returned_input,tokenized_input);
                            }
                            else
                            {
                                int val=checkforpasteventsexecute(tokenized_input);
                                char* line = getcmdpastevents(val,directory);
                                // printf("%s",line);
                                if(line!=NULL)
                                strcat(returned_input,line);
                                // printf("%d\n",val);
                            }
                            strcat(returned_input,"& ");
                                // exit(0)
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
                strcat(returned_input,";");
            }
            input=NULL;
    }
    // free(home_dir);
    // printf("%s",returned_input);
    return returned_input;
}

int checkforpastevents(char* tokenized_input)
{
    if (strncmp(tokenized_input,"pastevents purge",16)==0)
    {
        if(tokenized_input[17]=='\0'||tokenized_input[17]=='\n')
        return 0;
    }
    else if (strncmp(tokenized_input,"pastevents",10)==0)
    {
        if(tokenized_input[11]=='\0'||tokenized_input[11]=='\n')
        return 0;
    }
    return 1;
}

int parsedfully(char* input)
{
    char* returned_input=(char*)malloc(sizeof(char)*1024);
    returned_input[0]='\0';
    struct timeval start, end;
    long t_micros;
    float t_seconds;
    int i_t_seconds;
    int t_show=0;
    // double t_proc; 
    Directories directory;
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

        char* tokenized_input=calloc(4096,sizeof(char));
        char* saveptr_semicol;
        char* saveptr_amper;
        char* saveptr_space;
        char* token_semicol;
        char* token_amper;
        char* token_space;
        while (1)
        {
            token_semicol= strtok_r(input,";",&saveptr_semicol);
            if(token_semicol==NULL)
            {
                returned_input[strlen(returned_input)-1]='\0';
                break;
            }
            else
            {
                ampernospace=0;
                if (token_semicol[strlen(token_semicol)-1]=='\n'||token_semicol[strlen(token_semicol)-1]==' ')
                token_semicol[strlen(token_semicol)-1]='\0';
                if (strlen(token_semicol)>1)
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
                            if (!checkforpastevents(tokenized_input))
                            return 0;
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
                            if (!checkforpastevents(tokenized_input))
                            {
                                return 0;
                            }
                            strcat(returned_input,"& ");
                                // exit(0)
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
                strcat(returned_input,";");
            }
            input=NULL;
    }
    free(home_dir);
    // printf("%s",returned_input);
    return 1;    
}


void writeinfile(char* input,Directories* directory)
{
    // printf("a%sa",input);
    if(input[0]=='\0')
    return;
    char* filepath=malloc(sizeof(char)*256);
    int userid = getuid();
    errno=0; // setting errno to 0 to check for errors in getpwuid
    // struct passwd* userinfo = getpwuid(userid);
    char* cwd=NULL;
    cwd=getcwd(cwd,0);
    sprintf(filepath,"pastevents.txt");
    chdir(*(directory->home));
    // printf("%s",filepath);
    FILE* f1 =fopen(filepath,"ar+");
    size_t ret;
    unsigned char  buffer[4096];
    // f1 = fopen("/bin/sh", "rb");
    if (!f1) {

        perror("fopen");
        chdir(cwd);
        return;
    }
    ret = fread(buffer, sizeof(*buffer), 4096, f1);
    // printf("%d",ret);
    if (ret ==-1) {
        fprintf(stderr, "fread() failed: %zu\n", ret);
        chdir(cwd);
        // exit(EXIT_FAILURE);
        return;
    }
    int empty=0;
    if (buffer==NULL||ret==0)
    {
        // printf("Empty\n");
        empty=1;
        ret=1;
    }
    int count=1;
    char* lastline=buffer;
    for (int i=0;i<ret-1;i++)
    {
        if (buffer[i]=='\n')
        {
            lastline=&buffer[i+1];
            count++;
        }
    }
    buffer[ret]='\0';
    // printf("%s",lastline);
    // lastline[strlen(lastline)]='\0';
    // printf("%d",strncmp(input,lastline,strlen(lastline)-2));
    int cmpval=strlen(lastline)-1;
    if (input[strlen(input)-1]==' '||input[strlen(input)-1]=='\t')
    {
        input[strlen(input)-1]='\n';
    }
    if (input[strlen(input)-1]!='\n')
    {
        if(cmpval-1<strlen(input))
        {
            cmpval=strlen(input);
        }
    }
    else
    {
        if (cmpval<strlen(input))
        cmpval=strlen(input);
    }
    if (strncmp(input,lastline,cmpval)||empty==1)
    {
        if(count==15)
        {
            int k=0;
            // char* firstline=buffer;
            char* write;
            while (k<ret-1)
            {
                if (buffer[k]=='\n')
                {
                    write=&buffer[k+1];
                    break;
                }
                k++;
            }
            if (input[strlen(input)-1]==' '||input[strlen(input)-1]=='\t')
            input[strlen(input)-1]='\n';
            else
            {
                if (input[strlen(input)-1]!='\n')
                {
                    char* newinput=malloc(sizeof(char)*1024);
                    strcpy(newinput,input);
                    newinput[strlen(newinput)-1]='\n';
                    newinput[strlen(input)]='\0';
                    input=newinput;
                }
            }

            char* new_filestore=malloc(sizeof(char)*1024);
            strcpy(new_filestore,write);
            strcat(new_filestore,input);
            int t=strlen(new_filestore);
            fclose(f1);
            f1=fopen(filepath,"w");
            fwrite(new_filestore,sizeof(char),t,f1);
            fclose(f1);
        }
        else
        {
            char* new_filestore=malloc(sizeof(char)*1024);
            if (empty==0)
            strcpy(new_filestore,buffer);
            else
            new_filestore[0]='\0';
            // printf("a%ca",input[strlen(input)-2]);
            if (input[strlen(input)-1]==' '||input[strlen(input)-1]=='\t')
            {
                input[strlen(input)-1]='\n';
                strcat(new_filestore,input);
                // printf("a%sa",input);
            }
            else if(input[strlen(input)-1]!='\n')
            {
                strcat(new_filestore,input);
                strcat(new_filestore,"\n");
            }
            else
            {
                strcat(new_filestore,input);
            }
            // printf("%c",input[strlen(input)-1]);
            // printf("%s",input);
            
            // new_filestore()
            int t=strlen(new_filestore);
            fclose(f1);
            f1=fopen(filepath,"w");
            fwrite(new_filestore,sizeof(char),t,f1);
            fclose(f1);
        }
    }
    // printf("%d",count);
    // printf("%s",buffer);
    // fread(filepath);
    chdir(cwd);
    return;
    // printf("%s",filepath);   
}
#include "headers.h"

void recursedir(char* dirstart,char*curdir,int fflag,int eflag,int dflag,int* count,char* target,char** store,int print)
{
    int min;
    struct dirent **namelist;
    int n;
    char* dirprint;
    n = scandir(curdir, &namelist, NULL, alphasort);
    if (n == -1&&errno==ENOTDIR&&dflag==0) {
        if (*count==0)
        *count=*count+1;
        else
        {
            if (print)
            {
                if(*count==1)
                {
                    (*(store))=malloc(sizeof(char)*(strlen(dirstart)+1));
                    strcpy(*store,dirstart);
                }
                if(eflag==0)
                printf("\033[0;32m%s\033[0m\n",dirstart);
                (*count)++;
            }
        }
        return;
    }
    else if (n<0)
    {
        // printf("No permissions for task\n");
        return;
    }
    else
    {
        if (*count==0)
        *count=*count+1;
        else
        {
            if (fflag==0&&print==1)
            {
                if(*count==1)
                {
                    *store=malloc(sizeof(char)*(strlen(dirstart)+1));
                    strcpy(*store,dirstart);
                }
                if (eflag==0)
                printf("\033[0;34m%s\033[0m\n",dirstart);
                (*count)++;
            }
        }
    }
    while (n--) {
        if(n<0)
        {
            break;
        }
        if (namelist[n]->d_name[0]=='.')
        {
            // free(namelist[n]);
            // n--;
            continue;
        }
        chdir(curdir);
        // printf("x%sx\n",curdir);
        // fflush(stdout);
        char* temp5=NULL;
        temp5=getcwd(temp5,0);
        // printf("Hi");
        // fflush(stdout);
        char*diraddress=malloc(sizeof(char)*(strlen(dirstart)+strlen(namelist[n]->d_name)+3));
        strcpy(diraddress,dirstart);
        strcat(diraddress,"/");
        strcat(diraddress,namelist[n]->d_name);
        min=strlen(target);
        // printf("%s",namelist[n]->d_name);
        if (strncmp(target,namelist[n]->d_name,min))
        recursedir(diraddress,namelist[n]->d_name,fflag,eflag,dflag,count,target,store,0);
        else
        recursedir(diraddress,namelist[n]->d_name,fflag,eflag,dflag,count,target,store,1);
        // printf("%s\n", namelist[n]->d_name);
        // free(diraddress);
        chdir(temp5);
        // free(namelist[n]);
    }
    chdir(curdir);
    // free(namelist);
    // readdir(cur)
}


void seek(char* input,Directories* directory,int eflag)
{
    char* inputstored=input;
    // printf("%s\n",input);
    int dflag=0,fflag=0;
    int invalidflag=0;
    int index=0;
    for (int i=0;i<strlen(input);i++)
    {
        if (input[i]=='-')
        {
            while (i<strlen(input)-1)
            {
                if (input[i+1]=='d')
                {
                    dflag=1;
                }
                else if (input[i+1]=='e')
                {
                    if (eflag!=-1)
                    eflag=1;
                }
                else if (input[i+1]=='f')
                {
                    fflag=1;
                }
                else if (input[i+1]==' '||input[i+1]=='\t'||input[i+1]=='\n')
                {
                    index=i+2;
                    break;
                }
                else
                {invalidflag=1;}
                i++;    
            }
        }
    }
    if(invalidflag==1)
    {
        printf("Invalid flags!\n");
        return;
    }
    if (dflag==1&&fflag==1)
    {
        printf("Invalid flags!\n");
        return;
    }  
    if (eflag==-1)
    eflag=0;
    input=&input[index];
    // printf("%s",input);
    char* temp=NULL;
    temp=getcwd(temp,0);
    int count=0;
    char* store=malloc(sizeof(char)*256);
    char* target;
    index=0;
    while(input[index]!=' '&&input[index]!='\t'&&input[index]!='\n'&&input[index]!='\0')
    {
        index++;
    }
    input[index]='\0';
    target=&input[0];
    // printf("%s",target);
    input=&input[index+1];
    if (input[0]=='\0')
    {
        // chdir(input);
        // printf("%s",input);
        recursedir(".",temp,fflag,eflag,dflag,&count,target,&store,0);
        // peek(input,0,0,directory);
    }
    else if (input[0]=='~')
    {
        char* temp5=malloc(sizeof(char)*(strlen(*directory->home)+1));
        strcpy(temp5,*(directory->home));
        chdir(temp5);
        // strcat(temp5,"/");
        input=&input[1];
        while (input[0]==' '||input[0]=='\t'||input[0]=='\n')
        input=&input[1];
        if (input[strlen(input)-1]=='\t'||input[strlen(input)-1]=='\n'||input[strlen(input)-1]==' ')
        input[strlen(input)-1]='\0';
        strcat(temp5,input);
        // printf("%s\n",temp5);
        if (chdir(temp5)==-1)
        {
            printf("\033[0;31mInvalid Path\033[0m\n");
            // printf("Invalid Path\n");
            chdir(temp);
            return;
        } 
        // free(temp5);
        temp5=NULL;
        temp5=getcwd(temp5,0);
        char* temp6=malloc(sizeof(char)*256);
        strcpy(temp6,temp5);
        // printf("%s",temp5);
        recursedir(".",temp6,fflag,eflag,dflag,&count,target,&store,0);
    }
    else
    {
        if (input[strlen(input)-1]=='\t'||input[strlen(input)-1]=='\n'||input[strlen(input)-1]==' ')
        input[strlen(input)-1]='\0';
        if(chdir(input)==-1)
        {
            printf("\033[0;31mInvalid Path\033[0m\n");
            // printf("Invalid Path\n");
            return;
        }
        char* temp3=NULL;
        temp3=getcwd(temp3,0);
        // printf("%s",temp3);
        // chdir(temp);
        // printf("%s",input);
        // chdir(input);
        // char* temp2=NULL;
        // temp2=getcwd(temp2,0);
        recursedir(".",temp3,fflag,eflag,dflag,&count,target,&store,0);
        // chdir(temp);
        // free(temp3);
    }
    // free(*(store));
    // free(store);
    // after .. messes up
    int flagger=0;
    if (count==1)
    {
        printf("\033[0;31mNo match found\033[0m\n");
        // printf("No match found!\n");
    }
    if (count==2&&eflag==1)
    {
        struct stat dir;
        int readable=0,executable=0;
        // printf("%s",store);
        char* currrentdirrer=NULL;
        currrentdirrer=getcwd(currrentdirrer,0);
        char* fname=malloc(sizeof(char)*256);
        strcpy(fname,currrentdirrer);
        char* adder=&store[1];
        strcat(fname,adder);
        stat(fname,&dir);
        int val=dir.st_mode;
        int isdir=0;
        if(S_ISDIR(val)==1)
        {
            isdir=1;
        }
        if(isdir)
        {
            printf("\033[0;34m%s\033[0m\n",store);
        }
        else
        {
            printf("\033[0;32m%s\033[0m\n",store);
        }
        int factor=8;
        int perm[3];
        for (int i=0;i<3;i++)
        {
            // val=dir.st_mode%(factor);
            perm[2-i]=val%factor;
            val=val/8;
        }
        // char* permissions=(char*)malloc(sizeof(char)*15);
        for (int i=0;i<3;i++)
        {
            if (perm[i]>=4)
            {
                // printf("%d",perm[i]);
                perm[i]=perm[i]-4;
            }
            if (perm[i]>=2)
            {
                // read file
                readable=1;
                perm[i]=perm[i]-2;
            }
            if (perm[i]>=1)
            {
                executable=1;
            }
        }
        int checked_dir=1;
        if (fflag==0&&isdir)
        {
            // printf("Hello");
            if (executable==1)
            {
                flagger=1;
                chdir(fname);
                checked_dir=0;
            }
            else
            printf("\033[0;31mMissing permissions for task!\033[0m\n");
            // printf("Missing permissions for task!\n");
        }
        if (dflag==0&&readable==1&&isdir==0)
        {
            struct stat file1state;
            int returned;
            returned=stat(fname,&file1state);
            int fd1=open(fname,O_RDONLY);
            if (fd1==-1)
            {
                printf("\033[0;31mMissing permissions for task!\033[0m\n");
                return;
            }
            int a;
            long long int buffersize =256*256*5;
            char* str=(char*)malloc((buffersize+1)*sizeof(char));
            while (1)
            {
                a=read(fd1,str,buffersize);
                str[a]='\0';
                printf("%s",str);
                if(a<=0)
                {
                    printf("\n");
                    break;
                }
                // break;
            }   
        }
    }
    if (flagger==0)
    chdir(temp);
    if (count>2&&eflag==1)
    seek(inputstored,directory,-1);
    // free(temp);
}

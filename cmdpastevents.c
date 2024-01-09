#include "headers.h"

void exec_pastevents(Directories* directory)
{
    char* filepath=malloc(sizeof(char)*256);
    int userid = getuid();
    errno=0; // setting errno to 0 to check for errors in getpwuid
    struct passwd* userinfo = getpwuid(userid);
    sprintf(filepath,"pastevents.txt");
    // printf("%s",filepath);
    char* cwd=NULL; 
    cwd=getcwd(cwd,0);
    chdir(*(directory->home));
    FILE* f1 =fopen(filepath,"r");
    size_t ret;
    unsigned char  buffer[4096];
    // f1 = fopen("/bin/sh", "rb");
    if (!f1) {

        // printf("Empty\n");
        chdir(cwd);
        return;
    }
    ret = fread(buffer, sizeof(*buffer), 4096, f1);
    // printf("%d",ret);
    if (ret ==-1) {
        fprintf(stderr, "fread() failed: %zu\n", ret);
        // exit(EXIT_FAILURE);
        return;
    }
    int empty=0;
    if (buffer==NULL||ret==0)
    {
        // printf("Empty\n");
        empty=1;
        ret=1;
        chdir(cwd);
        return;
    }
    // int count=1;
    // char* lastline=buffer;
    // for (int i=0;i<ret-1;i++)
    // {
    //     if (buffer[i]=='\n')
    //     {
    //         lastline=&buffer[i+1];
    //         count++;
    //     }
    // }
    buffer[ret]='\0';
    printf("%s",buffer);
    fclose(f1);
    chdir(cwd);
    return;
}

void exec_pasteventspurge(Directories* directory)
{
    char* filepath=malloc(sizeof(char)*256);
    int userid = getuid();
    errno=0; // setting errno to 0 to check for errors in getpwuid
    struct passwd* userinfo = getpwuid(userid);
    sprintf(filepath,"pastevents.txt");
    // printf("%s",filepath);
    char* cwd=NULL; 
    cwd=getcwd(cwd,0);
    chdir(*(directory->home));
    FILE* f1 =fopen(filepath,"w");
    fclose(f1);   
    chdir(cwd);
}

char* getcmdpastevents (int line,Directories* directory)
{
    if (line>15)
    return NULL;
    char* filepath=malloc(sizeof(char)*256);
    int userid = getuid();
    errno=0; // setting errno to 0 to check for errors in getpwuid
    struct passwd* userinfo = getpwuid(userid);
    sprintf(filepath,"pastevents.txt");
    // printf("%s",filepath);
    char* cwd=NULL; 
    cwd=getcwd(cwd,0);
    chdir(*(directory->home));
    FILE* f1 =fopen(filepath,"ar+");
    size_t ret;
    unsigned char  buffer[4096];
    // f1 = fopen("/bin/sh", "rb");
    if (!f1) {

        perror("fopen");
        chdir(cwd);
        return NULL;
    }
    ret = fread(buffer, sizeof(*buffer), 4096, f1);
    // printf("%d",ret);
    if (ret ==-1) {
        fprintf(stderr, "fread() failed: %zu\n", ret);
        chdir(cwd);
        return NULL;
    }
    int empty=0;
    if (buffer==NULL||ret==0)
    {
        // printf("Empty\n");
        empty=1;
        ret=1;
        chdir(cwd);
        return NULL;
    }
    int count=1;
    for (int i=0;i<ret-1;i++)
    {
        if (buffer[i]=='\n')
        {
            // printf("a: %d b: %d\n",count,line);
            count++;
        }
    }
    line=count-line+1;
    if(line<1)
    {
        chdir(cwd);
        return NULL;
    }
    // return NULL;
    count=1;
    char* lastline=buffer;
    for (int i=0;i<ret-1;i++)
    {
        if (buffer[i]=='\n')
        {
            // printf("a: %d b: %d\n",count,line);
            if(count<line)
            {
                // printf("%d\n",count);
                lastline=&buffer[i+1];
            }
            count++;
        }
    }
    if (count<line)
    {  
        chdir(cwd);
        return NULL;
    }
    else
    {
        int index=0;
        while(lastline[index]!='\n')
        {
            index++;
        }
        lastline[index]='\0';
        // printf("%s\n",lastline);
        chdir(cwd);
        return lastline;
    }
    // buffer[ret]='\0';
}
#include "headers.h"

void warp(char* address,Directories* directory)
{
    // ensure emptry string gives address==NULL
    // check this is needed or not // address[strlen(address)-1]='\0';
    if (address==NULL||(address[0]=='~'&&address[1]=='\0'))
    {
        *(directory->owd)=NULL;
        *(directory->owd)=getcwd(*(directory->owd),0);
        // printf("Hi");
        printf("%s\n",*(directory->home));
        chdir(*(directory->home));
    }
    else if (address[0]=='~'&&address[1]=='/')
    {
        char* temping=NULL;
        temping=getcwd(temping,0);
        char* newaddress=malloc(sizeof(char)*(1024));
        newaddress[0]='\0';
        strcpy(newaddress,*(directory->home));
        // strcat(newaddress,"/");
        strcat(newaddress,&address[1]);
        // printf("%s\n",newaddress);
        if(chdir(newaddress)==-1)
        {
            printf("\033[0;31mInvalid Address\033[0m\n");
            chdir(temping);
            return;
        }
        *(directory->owd)=temping;
        char*idek=NULL;
        idek=getcwd(idek,0);
        printf("%s\n",idek);
    }
    else if (address[0]=='-'&&address[1]=='\0')
    {
        if (*(directory->owd)!=NULL)
        {
            char* temp=NULL;
            temp=getcwd(temp,0);
            // printf("Hi");
            chdir(*(directory->owd));
            printf("%s\n",*(directory->owd));
            *(directory->owd)=temp;
        }
        else
        printf("\033[0;31mOLDPWD not set\033[0m\n");
    }
    else
    {
        char* temp2=NULL;
        temp2=getcwd(temp2,0);
        // printf("%s",address);
        int ret_val=chdir(address);
        if (ret_val!=-1)
        {
            *(directory->owd)=temp2;
            temp2=NULL;
            temp2=getcwd(temp2,0);
            printf("%s\n",temp2);
        }
        else
        {
            printf("\033[0;31mInvalid Address\033[0m\n");
            free(temp2);
        }
    }
    // else if (address=="-")
    // {
    //     char* temp = *(directory->owd);
    //     *(directory->owd)=*(presentWD);
    //     *(presentWD)=temp;
    //     printf("%s\n",temp);
    // }
    // else if (address[0]=="/")
    // {
    //     printf("Hi\n");
    // }
    // else
    // {
    //     char* newaddress=(char*)malloc(sizeof(char*)(strlen(directory->home)+strlen(address)+2));
    //     strcpy(newaddress,directory->home);
    //     newaddress[strlen(directory->home)]='/';
    //     newaddress[strlen(directory->home)+1]='\0';
    //     strcat(newaddress,address);
    //     printf("%s",newaddress);
    // }
    return;
}
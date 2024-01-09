#include "headers.h"

void peek(char* address,int lflag,int aflag,Directories* directory)
{
    // DIR* main_dir;
    // struct dirent* dir_entry;
    int isexec=0;
    // printf("%c%c",address[0],address[1]);
    // printf("%ld",strlen(address));
    int dflag=0,fflag=0;
    int eflag=0;
    int invalidflag=0;
    int index=0;
    // printf("a%sa\n",address);
    int temp_len=strlen(address);
    for (int i=0;i<temp_len;i++)
    {
        if (address[i]=='-')
        {
            while (i<strlen(address)-1)
            {
                if (address[i+1]=='a')
                {
                    aflag=1;
                }
                else if (address[i+1]=='l')
                {
                    lflag=1;
                }
                else if (address[i+1]=='\0')
                {
                    address=NULL;
                    break;
                }
                else if (address[i+1]==' '||address[i+1]=='\t'||address[i+1]=='\n')
                {
                    if (address[i+2]=='\0')
                    {
                        address=NULL;
                        break;
                    }
                    index=i+2;
                    break;
                }
                else
                {invalidflag=1;}
                i++;  
                if (i==strlen(address)-1&&index==0)
                {
                    address=NULL;
                    break;
                }
            }
        }
    }
    if(invalidflag==1)
    {
        printf("\033[0;31mInvalid flags!\033[0m\n");
        return;
    }
    // if (index==0)
    // {
    //     address=".";
    // }
    // else
    // {
    if (address!=NULL)
    {
        while (address[index]==' '||address[index]=='\t'||address[index]=='\n')
        index++;
        address=&address[index];
    }
    else
    {
        address=".";
    }

    printf("%sa\n",address);
    struct dirent **directlist;
    int count;
    char* pathname;
    int n;
    int it=0;
    struct stat dir;
    char* temp=NULL;
    temp=getcwd(temp,0);
    char* originaldir=temp;
    // printf("%s",address);
    if (strlen(address)!=0)
    {
        if (address[strlen(address)-1]==' '||address[strlen(address)-1]=='\t'||address[strlen(address)-1]=='\n')
        address[strlen(address)-1]='\0';
    }
    // printf("%s",address);
    if (address==NULL||(address[0]=='~'&&(address[1]==' '||address[1]=='/'||address[1]=='\0')))
    {
        // main_dir=opendir(*(directory->home));   
        
        pathname=malloc(sizeof(char)*1024);  
        strcpy(pathname,*(directory->home));
        if(address[1]=='/')
        strcat(pathname,&address[1]);
        // printf("%s",pathname);
        n = scandir(pathname, &directlist, NULL, alphasort);
        // printf("%d\n",chdir(pathname));
        // printf("Hi");  
        // chdir(pathname);
        if (chdir(pathname)==-1)
        {
            printf("\033[0;31mInvalid Address\033[0m\n");
            // printf("Invalid Path\n");
            // chdir(originaldir);
            return;
        }
    }
    else if (strlen(address)==0)
    {
        temp=NULL;
        temp=getcwd(temp,0);
        pathname=temp;
        n = scandir(temp, &directlist, NULL, alphasort);
    }
    else
    {
        // main_dir=opendir(address);
        n = scandir(address, &directlist, NULL, alphasort);
        pathname=address;
        if (chdir(pathname)==-1)
        {
            printf("\033[0;31mInvalid Address\033[0m\n");
            // chdir(originaldir);
            return;
        }
    }
    // char* temp=NULL;
    // temp=getcwd(temp,0);
    // chdir(temp);
    // sleep(1);
    // char* pathing=malloc(sizeof(char)*(strlen(path)+3));
    // strcat(pathing,"./");
    // path[strlen(path)-1]='\0';
    // strcpy(pathing,path);
    // printf("%s",pathing);
    // main_dir = opendir(pathing);
    // main_dir=opendir("..");
    // struct dirent **directlist;
    if (n == -1)
    {
        perror("scandir failed");
        chdir(temp);
        return;
        // exit(EXIT_FAILURE);
    }
    // printf("%d %d %d %d\n",S_IEXEC,S_IFDIR,(33204 & S_IFMT),S_IFREG);
    char time_str[256] = "";
    time_t now;
    struct tm tmfile, tmnow;
    // chdir("..");
    stat(pathname,&dir);
    long total=0;
    // total=total+dir.st_blocks;
    // chdir(pathname);
    while (it<n)
    {
        if (aflag==0&&directlist[it]->d_name[0]=='.')
        {
            it++;
            continue;
        }
        stat(directlist[it]->d_name,&dir);
        total=total+dir.st_blocks;
        it++;
    }
    it=0;
    if (lflag==1)
    printf("total %ld\n",total);
    while (it<n) 
    {
        // char* directpath=malloc(sizeof(char)*(strlen(pathname)+strlen(directlist[it]->d_name)+2));
        // directpath[0]='\0';
        // strcpy(directpath,pathname);
        // directpath[strlen(directpath)]='/';
        // directpath[strlen(directpath)+1]='\0';
        // strcat(directpath,directlist[it]->d_name);
        // printf("%s",directpath);
        
        // now need to find out the flags to check if exec if dir, etc.
        stat(directlist[it]->d_name,&dir);
        int val=dir.st_mode;
        int factor=8;
        int perm[3];
        for (int i=0;i<3;i++)
        {
            // val=dir.st_mode%(factor);
            perm[2-i]=val%factor;
            val=val/8;
        }
        char* permissions=(char*)malloc(sizeof(char)*15);
        if (directlist[it]->d_type==4)
        strcpy(permissions,"d");
        else if (directlist[it]->d_type==8)
        strcpy(permissions,"-");
        for (int i=0;i<3;i++)
        {
            if (perm[i]>=4)
            {
                strcat(permissions,"w");
                perm[i]=perm[i]-4;
            }
            else
            {
                strcat(permissions,"-");
            }
            if (perm[i]>=2)
            {
                strcat(permissions,"r");
                perm[i]=perm[i]-2;
            }
            else
            {
                strcat(permissions,"-");
            }
            if (perm[i]>=1)
            {
                strcat(permissions,"x");
                perm[i]=perm[i]-1;
                if (i==0&&directlist[it]->d_type!=4)
                isexec=1;
            }
            else
            {
                strcat(permissions,"-");
            }
        }
        if (lflag==1&&(aflag||directlist[it]->d_name[0]!='.'))
        {
            // printf("%d a\n",dir.type);
            now = time (NULL);
            localtime_r (&dir.st_mtime, &tmfile);
            localtime_r (&now, &tmnow);    
            if (tmfile.tm_year == tmnow.tm_year)
            {   
                strftime (time_str, sizeof (time_str), "%b %e %H:%M",&tmfile);   
            }
            else
            {
                strftime (time_str, sizeof (time_str), "%b %e  %Y",&tmfile);
            }
            struct passwd* userinfo;
            struct group* grpinfo;
            userinfo = getpwuid(dir.st_uid);
            grpinfo = getgrgid(dir.st_gid);
            if (errno!=0&&userinfo==NULL)
            {
                perror("Error: getpwuid function failed\n");
                chdir(originaldir);
                return;
            }
            // struct timespec st_mtim = dir.st_mtim;
            printf("%s %lu %s %s %ld\t %s ",permissions,dir.st_nlink,userinfo->pw_name,grpinfo->gr_name,dir.st_size,time_str);
        }
        free(permissions);
        // if ((dir.st_mode & S_IFMT)==S_IEXEC)
        // printf("%d ",dir.st_mode);
        //color code here depending on type
        // the below can be used to identify a directory, then for files check exec perm for users
        // printf("%d ",directlist[it]->d_type);
        if (aflag||directlist[it]->d_name[0]!='.')
        {
            if (isexec==1)
            {
                printf("\033[0;32m%s\033[0m\n",directlist[it]->d_name);
                isexec=0;
            }
            else if (directlist[it]->d_type==4)
            {
                printf("\033[0;34m%s\033[0m\n",directlist[it]->d_name);
            }
            else
            printf("%s\n", directlist[it]->d_name);
        }

        free(directlist[it]);
        it++; 
    }
    free(directlist);
    // dir_entry=readdir(main_dir);
    // while (dir_entry!=NULL)
    // {
    //     printf("%s\n",dir_entry->d_name);
    //     dir_entry=readdir(main_dir);
    // }
    // printf("%d",a);
    chdir(originaldir);
    return;
}
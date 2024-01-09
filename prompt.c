#include "headers.h"

void prompt(char* home_dir,int home_len,int flag,struct t_cmd* fg_cmd) {
    // Do not hardcode the prmopt
    struct utsname sysname;
    int sysname_ret = uname(&sysname);
    if (sysname_ret==-1&&errno==EFAULT)
    {
        perror("Error: Bad Address in uname function\n");
        return;
    }
    int userid = getuid();
    errno=0; // setting errno to 0 to check for errors in getpwuid
    struct passwd* userinfo = getpwuid(userid);
    if (errno!=0&&userinfo==NULL)
    {
        perror("Error: getpwuid function failed\n");
        return;
    }
    char* cwd_buffer=NULL;
    cwd_buffer=getcwd(cwd_buffer,0);
    if (cwd_buffer==NULL)
    {
        if (errno==ENOMEM)
        {
            perror("Error: Pathname too large\n"); 
        }
        else
        {
            perror("Error: get_cwd function failed\n");
        }
        return;
    }
    // but if they intentionally give a very large file name to problem ho jaega na? idk
    else
    {
        if (flag==1)
        {
            if(strncmp(home_dir,cwd_buffer,home_len)==0)
            {
                char* cwd_wrt_home=&cwd_buffer[home_len];
                printf("\033[0;32m%s@%s\033[0m:\033[0;34m~%s\033[0m %s: %d>",userinfo->pw_name,sysname.nodename,cwd_wrt_home,fg_cmd->cmd,fg_cmd->time); 
            }  
            else
            {
                printf("\033[0;32m%s@%s\033[0m:\033[0;34m%s\033[0m %s: %d>",userinfo->pw_name,sysname.nodename,cwd_buffer,fg_cmd->cmd,fg_cmd->time); 
            }   
        }
        else
        {
            if(strncmp(home_dir,cwd_buffer,home_len)==0)
            {
                char* cwd_wrt_home=&cwd_buffer[home_len];
                printf("\033[0;32m%s@%s\033[0m:\033[0;34m~%s\033[0m>",userinfo->pw_name,sysname.nodename,cwd_wrt_home); 
            }  
            else
            {
                printf("\033[0;32m%s@%s\033[0m:\033[0;34m%s\033[0m>",userinfo->pw_name,sysname.nodename,cwd_buffer); 
            }
        }
    }
    free(cwd_buffer);
}

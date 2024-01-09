#include "headers.h"

void iMan(char* input) {
    
    struct addrinfo hints, *res;
    int socket_fd;
    if (input[strlen(input)-1]==' '||input[strlen(input)-1]=='\t'||input[strlen(input)-1]=='\n')
    input[strlen(input)-1]='\0';
    // char input2[1000];
    for (int i=0;i<strlen(input)-1;i++)
    {
        if (input[i]==' ')
        input[i]='+';
    }
    char buf[10000];
    int countByte;
    // printf("a%sa\n",input);    
    memset(&hints, 0,sizeof hints);
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo("man.he.net","80", &hints, &res);
    socket_fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    connect(socket_fd,res->ai_addr,res->ai_addrlen);
    char header[1000];
    //  = "GET /?topic=sleep&section=all HTTP/1.1\r\nHost: man.he.net\r\n\r\n";
    sprintf(header,"GET /?topic=%s&section=all HTTP/1.1\r\nHost: man.he.net\r\n\r\n",input);
    if (send(socket_fd,header,strlen(header),0)<0)
    {
        printf("No such page found\n");
        return;
    }
    int n;
    int print_flag=0;
    int break_flag=0;
    int no_flag=0;
    while ((n=read(socket_fd,buf,10000))>0)
    {
        buf[countByte]='\0';
        for (int i=0;i<n;i++)
        {
            if (strncmp(&buf[i],"AUTHOR\n",7)==0)
            break;
            if (print_flag==1||strncmp(&buf[i],"NAME\n",5)==0)
            {
                no_flag=1;
                for (int j=i;j<n;j++)
                {
                    if(strncmp(&buf[j],"AUTHOR\n",7)==0)
                    buf[j]='\0';
                    break_flag=1;
                }
                printf("%s",&buf[i]);
                print_flag=1;
                if (break_flag==1)
                {
                    break;
                }
            }
        }
        // countByte = recv(socket_fd,buf,10000,0);
    }
    if (print_flag==0&&n<0)
    {
        printf("Error couldn't read the data in the page");
    }
    else if (no_flag==0)
    {
        printf("\e[0;31mERROR\n\tNo such command\033[0m\n");
        // printf("ERROR\n\tNo such command\n");
    }
    else
    {
        printf("\n");
    }
    close(socket_fd);
    return;
}
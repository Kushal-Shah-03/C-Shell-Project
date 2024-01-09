#include "headers.h"

void die(const char *s) {
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

/**
 * Enable row mode for the terminal
 * The ECHO feature causes each key you type to be printed to the terminal, so you can see what you’re typing.
 * Terminal attributes can be read into a termios struct by tcgetattr().
 * After modifying them, you can then apply them to the terminal using tcsetattr().
 * The TCSAFLUSH argument specifies when to apply the change: in this case, it waits for all pending output to be written to the terminal, and also discards any input that hasn’t been read.
 * The c_lflag field is for “local flags”
*/
void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

/**
 * stdout and stdin are buffered we disable buffering on that
 * After entering in raw mode we read characters one by one
 * Up arrow keys and down arrow keys are represented by 3 byte escape codes
 * starting with ascii number 27 i.e. ESC key
 * This way we interpret arrow keys
 * Tabs are usually handled by the term, but here we are simulating tabs for the sake of simplicity
 * Backspace move the cursor one control character to the left
 * @return
*/
void neonate(int max_time) {
    char *inp = malloc(sizeof(char) * 100);
    char c;
    struct timeval start, end;
    long t_micros;
    float t_seconds;
    gettimeofday(&start, NULL);
    int ret=fork();
    if (ret<0)
    printf("Error in fork\n");
    if (ret==0)
    {
        // setpgid(0,0);
        // printf("wth");
        gettimeofday(&start,NULL);
        while(1)
        {
                gettimeofday(&end, NULL);
                t_micros = (((end.tv_sec - start.tv_sec) * 1000000) + end.tv_usec) - (start.tv_usec);
                t_seconds=t_micros/1000000.0;
                if (t_seconds>=max_time)
                {
                    gettimeofday(&start,NULL);
                    // disableRawMode();
                    // int maxpid=-1;
                    // struct dirent *entry = NULL;
                    // DIR *dp = NULL;
                    // dp = opendir("/proc");
                    // if (dp != NULL)
                    // {
                    //     while ((entry = readdir(dp)))
                    //     {
                    //         if (isdigit(entry->d_name[0]))
                    //         {
                    //             int pid=atoi(entry->d_name);
                    //             if (pid>maxpid)
                    //             maxpid=pid;
                    //         }
                    //         // printf ("%s\n", entry->d_name);
                    //     }
                    // }
                    // closedir(dp);
                    char buffer[4096];
                    FILE* procFile = fopen("/proc/loadavg", "r");
                    if (!procFile)
                    {
                        printf("\031[0m;Invalid Permissions\033[0m\n");
                        // printf("Invalid Permissions\n");
                        // return NULL;
                        return;
                    }
                    (fgets(buffer, 4096, procFile));
                    char* print;
                    // fopen("/proc/loadavg",)
                    for (int i=0;i<strlen(buffer);i++)
                    {
                        if (buffer[i]==' ')
                        print=&buffer[i+1];
                    }
                    printf("%s",print);
                    fflush(stdout);
                    // enableRawMode();
                    // fflush(stdout);
                }
        }
        exit(0);
    }
    setpgid(ret,getpid());
    while (1) {
        // printf("%d\n",ret);
        setbuf(stdout, NULL);
        enableRawMode();
        // printf("Prompt>");
        memset(inp, '\0', 100);
        int pt = 0;
        while (read(STDIN_FILENO, &c, 1) == 1) {
                // gettimeofday(&end, NULL);
                // t_micros = (((end.tv_sec - start.tv_sec) * 1000000) + end.tv_usec) - (start.tv_usec);
                // t_seconds=t_micros/1000000.0;
                // if (t_seconds>=4)
                // {
                //     gettimeofday(&start,NULL);
                //     disableRawMode();
                //     printf("Hi");
                //     enableRawMode();
                //     // fflush(stdout);
                // }
            if (iscntrl(c)) {
                if (c == 10) break;
                else if (c == 27) {
                    char buf[3];
                    buf[2] = 0;
                    if (read(STDIN_FILENO, buf, 2) == 2) { // length of escape code
                        // printf("\rarrow key: %s", buf);
                    }
                } else if (c == 127) { // backspace
                    if (pt > 0) {
                        if (inp[pt-1] == 9) {
                            for (int i = 0; i < 7; i++) {
                                printf("\b");
                            }
                        }
                        inp[--pt] = '\0';
                        printf("\b \b");
                    } 
                }else if (c == 9) { // TAB character
                    // inp[pt++] = c;
                    // for (int i = 0; i < 8; i++) { // TABS should be 8 spaces
                    //     printf(" ");
                    // }
                } else if (c == 4) {
                    exit(0);
                } else {
                    // printf("%d\n", c);
                }
            } else {
                if (c=='x')
                {
                    kill(ret,SIGKILL);
                    disableRawMode();
                    return;
                } 
                // inp[pt++] = c;
                // printf("%c", c);
            }
        }
        disableRawMode();

        // printf("\nInput Read: [%s]\n", inp);
    }
    if (ret==0)
    kill(-1*ret,SIGKILL);
    return;
}

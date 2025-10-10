#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <errno.h>

int main()
{
    int fed;
    char buff[101];
    int len, i, byt_writ;
    char* fifo_name = "fifo";
    signal(SIGPIPE, SIG_IGN);
    srand(time(NULL));

    fed = open("fifo", O_WRONLY);
    if (fed == -1)
    {
        perror("open");
        return 1;
    }

    while (1)
    {
        len = rand() % 100 + 1;

        for (i = 0; i < len; i++)
        {
            buff[i] = 'A' + rand() % 26;
        }
        buff[len] = '\0';

        printf("sent '%s'\n", buff);

        byt_writ = write(fed, buff, len + 1);
        if (byt_writ < 0)
        {
            if (errno == EPIPE) 
            {
                printf("server closed FIFO.\n");
                break;
            }
            else
            {
                perror("write in client");
                break;
            }
        }
        usleep(100000);
    }
    close(fed);
    return 0;
}
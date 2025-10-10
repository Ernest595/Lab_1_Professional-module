#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

int main()
{
    char* fifo_name = "fifo";
    int fed;
    char buff[101];
    int length_counts[101];
    int i, byt_read, done;

    for (i = 0; i < 101; i++)
    {
        length_counts[i] = 0;
    }

    mkfifo(fifo_name, 0666);

    fed = open(fifo_name, O_RDONLY);
    if (fed == -1)
    {
        perror("open fifo in server");
        return 1;
    }

    done = 0;
    while (!done)
    {
        byt_read = read(fed, buff, sizeof(buff) - 1);
        if (byt_read > 0)
        {
            buff[byt_read] = '\0';
            length_counts[byt_read]++;
            printf("%s \n", buff, byt_read);

            if (length_counts[byt_read] >= 5)
            {
                printf("Length %d encountered 5 times.\n", byt_read);
                break;
            }
        }
    }

    close(fed);
    unlink(fifo_name);

    return 0;
}
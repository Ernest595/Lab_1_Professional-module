#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
    int pid_arr[2];
    pid_t pid;
    char buffer[100];
    char message[] = "Hello world!";
    size_t ett;

    if (pipe(pid_arr) == -1)
    {
        perror("pipe");
        return 1;
    }

    pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        close(pid_arr[0]);

        ett = write(pid_arr[1], message, strlen(message) + 1);
        if (ett == -1)
        {
            perror("write_ett");
            return 1;
        }
    }
    else
    {
        close(pid_arr[1]);

        ett = read(pid_arr[0], buffer, sizeof(buffer));
        if (ett == -1)
        {
            perror("read_ett");
            return 1;
        }

        printf("%s\n", buffer);
    }

    return 0;
}
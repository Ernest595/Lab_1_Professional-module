#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int geceded(int a, int b)
{
    int t;
    while (b != 0)
    {
        t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int isprimer(int n)
{
    int i;
    if (n < 2) return 0;
    for (i = 2; i * i <= n; i++)
    {
        if (n % i == 0) return 0;
    }
    return 1;
}

int main(void)
{
    int p[2];
    int c[2];
    pid_t pid;
    size_t err;
    int status, recived_n, result_count, i, count, n;

    if ((pipe(p) == -1) || (pipe(c) == -1))
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
        close(p[1]);
        close(c[0]);

        while (1)
        {
            err = read(p[0], &recived_n, sizeof(recived_n));
            if (err == -1)
            {
                perror("read");
                break;
            }
            if (err == 0) break;

            result_count = 0;
            for (i = 1; i < recived_n; i++)
            {
                if (gcd(i, recived_n) == 1)
                {
                    result_count++;
                }
            }

            err = write(c[1], &result_count, sizeof(result_count));
            if (err == -1)
            {
                perror("write");
                break;
            }
        }
    }
    else
    {
        close(p[0]);
        close(c[1]);

        count = 0;
        srand(getpid());

        while (count < 50)
        {
            n = rand() % 1000 + 1;
            err = write(p[1], &n, sizeof(n));
            if (err == -1)
            {
                perror("write");
                break;
            }

            err = read(c[0], &result_count, sizeof(result_count));
            if (err == -1)
            {
                perror("read");
                break;
            }

            if (result_count == n - 1)
            {
                count++;
                printf("Parent: (count=%d)\n", n, count);
            }
        }
    }

    return 0;
}
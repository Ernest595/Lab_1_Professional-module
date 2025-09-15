#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_prime(unsigned char x)
{
    int i;
    if (x < 2)
        return 0;
    for (i = 2; i * i <= x; i++)
    {
        if (x % i == 0)
            return 0;
    }
    return 1;
}

int main(int argc, char* argv[])
{
    FILE* file;
    int i, rap;
    unsigned char res8, res32, b, has_prime;
    unsigned char buffer[4];
    unsigned int count, value, mask;

    if (argc < 3)
    {
        printf("%s <source> and <destination>\n", argv[0]);
        return 1;
    }

    file = fopen(argv[1], "rb");
    if (!file)
    {
        printf("Invalid file.\n");
        return 1;
    }

    if (strcmp(argv[2], "xor8") == 0)
    {
        res8 = 0;
        while (fread(&b, 1, 1, file) == 1)
            res8 ^= b;
        printf("%u\n", res8);
    }
    else if (strcmp(argv[2], "xorodd") == 0)
    {
        res32 = 0;
        while ((rap = fread(buffer, 1, 4, file)) == 4)
        {
            has_prime = 0;
            for (i = 0; i < 4; i++)
            {
                if (is_prime(buffer[i]))
                {
                    has_prime = 1;
                    break;
                }
            }
            if (has_prime)
            {
                value = buffer[0] | (buffer[1] << 8) | (buffer[2] << 16) | (buffer[3] << 24);
                res32 ^= value;
            }
        }
        printf("%u\n", res32);
    }
    else if (strcmp(argv[2], "mask") == 0 && argc >= 4)
    {
        mask = strtoul(argv[3], NULL, 16);
        //count = 0;
        while ((rap = fread(buffer, 1, 4, file)) == 4)
        {
            value = buffer[0] | (buffer[1] << 8) | (buffer[2] << 16) | (buffer[3] << 24);
            if ((value & mask) == mask)
                count++;
        }
        printf("%u\n", count);
    }

    fclose(file);
    return 0;
}
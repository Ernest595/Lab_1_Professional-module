#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    FILE* rec, * set;
    size_t bytes;
    char buffer[4096];
    if (argc != 3)
    {
        printf("Invalid arguments");
        return 1;
    }

    rec = fopen(argv[1], "rb");
    if (!rec)
    {
        printf("The file did not open\n");
        return 1;
    }

    set = fopen(argv[2], "wb");
    if (!set)
    {
        fclose(rec);
        printf("The target file has not opened\n");
        return 1;
    }

    while ((bytes = fread(buffer, 1, sizeof(buffer), rec)) > 0)
    {
        fwrite(buffer, 1, bytes, set);
    }

    fclose(rec);
    fclose(set);
    return 0;
}
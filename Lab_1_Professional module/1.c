#include <stdio.h>

int main(int argc, char* argv[]) 
{
    FILE* file;
    int nap = 0;
    unsigned char i;
    unsigned char buf[] = { 3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5 };
    unsigned char buffer[4];

    if (argc != 2) 
    { 
        return -1; 
    }

    file = fopen(argv[1], "wb");
    if (file == NULL) 
    { 
        return -2; 
    }

    if (fwrite(buf, sizeof(unsigned char), 11, file) != 11) 
    {
        fclose(file);
        return -3;
    }

    fclose(file);

    file = fopen(argv[1], "rb");
    if (file == NULL) 
    { 
        return -2; 
    }

    printf("Reading a file by bytes:\n");

    while (fread(&i, sizeof(unsigned char), 1, file) == 1) 
    {
        printf("\nByte: %d\nFILE fields:\nFill: %p\n", i, file->_Placeholder);
    }

    if (ferror(file)) 
    {
        fclose(file);
        return -4;
    }

    fclose(file);

    file = fopen(argv[1], "rb");
    if (file == NULL) 
    { 
        return -2; 
    }

    if (fseek(file, 3, SEEK_SET) != 0) 
    {
        fclose(file);
        return -5;
    }

    if (fread(buffer, sizeof(unsigned char), 4, file) != 4) 
    {
        fclose(file);
        return -6;
    }

    printf("\n After fseek:\n");
    for (int i = 0; i < 4; i++) 
    {
        printf("%d ", buffer[i]);
    }
    printf("\n");

    fclose(file);

    return 0;
}

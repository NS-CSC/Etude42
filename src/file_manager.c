#include <stdio.h>
void create_file(const char* filename)
{
    FILE* file;
    int status;  // 読み込みできたかのフラグ
    status = 0;
    file = fopen(filename, "r+");
    if (file != NULL)
    {
        fclose(file);
        return;
    }
    else
    {
        status = 1;
    }

    if (status == 1)
    {
        file = fopen(filename, "w");
        if (file == NULL)
        {
            printf("Failed to create file\n");
        }
    }
    fclose(file);
    return;
}
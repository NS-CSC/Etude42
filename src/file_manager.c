#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

  
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
    } else
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
// 読み込み権限があるか
int check_read_permission(const char* filename)
{
    if (access(filename, R_OK) == -1)
    {
        if (errno == EACCES)
        {
            fprintf(stderr, "ERROR: '%s' への読み込み権限がありません\n",
                    filename);

            return -1;

        } else if (errno == ENOENT)
        {
            fprintf(stderr, "ERROR: ファイル '%s' が存在しません\n", filename);

            return -2;
        } else
        {
            fprintf(stderr, "ERROR: %s\n", strerror(errno));

            return -3;
        }
    }
    // 読み込み権限あり
    return 0;
}

// 書き込み権限があるか
int check_write_permission(const char* filename)
{
    if (access(filename, W_OK) == -1)
    {
        if (errno == EACCES)
        {
            fprintf(stderr, "ERROR:'%s'への書き込み権限がありません\n",
                    filename);

            return -1;
        } else if (errno == ENOENT)
        {
            fprintf(stderr, "ERROR: ファイル '%s' が存在しません\n", filename);

            return -2;
        } else
        {
            fprintf(stderr, "ERROR: %s\n", strerror(errno));

            return -3;
        }
    }
    // 書き込み権限あり
    return 0;
}

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

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

// ファイルのポインタを返す関数
FILE* get_file_pointer(const char* file_path)
{
    FILE* file;

    // ファイルが存在して正常に開けた時の処理
    file = fopen(file_path, "r+");
    if (file != NULL)
    {
        return file;
    }

    // ファイルが存在して新しく作成する処理
    file = fopen(file_path, "a+");
    if (file != NULL)
    {
        return file;
    }

    printf("Error: %s", strerror(errno));
    return NULL;
}

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

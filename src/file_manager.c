#include "file_manager.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "display.h"

#define LIMIT_LINE_LEN 10000

FILE *get_file_pointer(const char *file_path);
// ファイルのポインタを返す関数
int read_file(const char *file_path);
// ファイルのパスを引数に指定するとファイルを読み込む関数
int save_file(const char *file_path, const char *data);
// ファイルをセーブする関数

// // 読み込み権限があるか
// int check_read_permission(const char *filename)
// {
//     if (access(filename, R_OK) == -1)
//     {
//         if (errno == EACCES)
//         {
//             fprintf(stderr, "ERROR: '%s' への読み込み権限がありません\n", filename);

//             return -1;

//         } else if (errno == ENOENT)
//         {
//             fprintf(stderr, "ERROR: ファイル '%s' が存在しません\n", filename);

//             return -2;
//         } else
//         {
//             fprintf(stderr, "ERROR: %s\n", strerror(errno));

//             return -3;
//         }
//     }
//     // 読み込み権限あり
//     return 0;
// }

// // 書き込み権限があるか
// int check_write_permission(const char *filename)
// {
//     if (access(filename, W_OK) == -1)
//     {
//         if (errno == EACCES)
//         {
//             fprintf(stderr, "ERROR:'%s'への書き込み権限がありません\n", filename);

//             return -1;
//         }

//         else if (errno == ENOENT)
//         {
//             fprintf(stderr, "ERROR: ファイル '%s' が存在しません\n", filename);

//             return -2;
//         }

//         else
//         {
//             fprintf(stderr, "ERROR: %s\n", strerror(errno));

//             return -3;
//         }
//     }
//     // 書き込み権限あり
//     return 0;
// }

// ファイルのポインタを返す関数
FILE *get_file_pointer(const char *file_path)
{
    FILE *file;

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

// ファイルのパスを引数に指定するとファイルを読み込む関数
int read_file(const char *file_path)
{
    FILE *fp;
    char *line; // getline用のバッファ
    char *content[LIMIT_LINE_LEN]; // とりえず10000行読み込める
    size_t len;
    ssize_t read;
    int count;

    line = NULL;
    len = 0;
    count = 0;

    fp = get_file_pointer(file_path);

    if (fp == NULL)
    {
        fprintf(stderr, "Failed to open or create file: %s\n", file_path);
        return -1;
    }

    while ((read = getline(&line, &len, fp)) != -1)
    {
        content[count] = strdup(line);
        count++;
    }

    free(line);

    const int close_result = fclose(fp);

    if (close_result != 0)
    {
        fprintf(stderr, "Error closing file %s: %s\n", file_path, strerror(errno));
        return -1;
    }

    // render_screen(content)
    // 描画については後にdisplay.cができてから
    return 0;
}

// ファイルをセーブする関数
int save_file(const char *file_path, const char *data)
{
    FILE *fp = get_file_pointer(file_path);

    if (fp == NULL)
    {
        fprintf(stderr, "Failed to open or create file: %s\n", file_path);
        return -1;
    }

    const int write_result = fprintf(fp, "%s", data);

    // 書き込みが成功したか確認
    if (write_result < 0)
    {
        fprintf(stderr, "Error writing to file %s: %s\n", file_path, strerror(errno));

        const int close_result = fclose(fp);

        // クローズが成功したか確認
        if (close_result != 0)
        {
            fprintf(stderr, "Error closing file %s: %s\n", file_path, strerror(errno));
    
            return -1;
        }

        return -1;
    }

    const int close_result = fclose(fp);

    // クローズが成功したか確認
    if (close_result != 0)
    {
        fprintf(stderr, "Error closing file %s: %s\n", file_path, strerror(errno));

        return -1;
    }

    return 0;
}

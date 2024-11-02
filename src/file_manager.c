#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LEN 256    // 一行の長さの最大値
#define LINE_LEN 1000  // ファイルの行数

typedef struct FileInfo
{
    char* content[LINE_LEN];
    int file_len;
    int is_exist;  // 存在: 1 存在しない: -1
} FileInfo;

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

FileInfo read_file(const char* file_path)
{
    FILE* fp;
    char* content[LINE_LEN];
    char line_buffer[MAX_LEN];
    char* result;
    int file_len;
    file_len = 0;
    FileInfo file_info;
    file_info.file_len = 0;
    file_info.is_exist = -1;
    fp = get_file_pointer(file_path);
    if (fp == NULL)
    {
        fprintf(stderr, "Failed to open or create file: %s\n", file_path);
        return file_info;
    }

    while ((result = fgets(line_buffer, MAX_LEN, fp)) != NULL)
    {
        char* p = (char*)malloc(sizeof(char*) * sizeof(line_buffer));
        strcpy(p, result);
        file_info.content[file_len] = p;
        file_len++;
    }

    file_info.is_exist = 1;
    file_info.file_len = file_len;

    const int close_result = fclose(fp);

    if (close_result != 0)
    {
        fprintf(stderr, "Error closing file %s: %s\n", file_path,
                strerror(errno));
        return file_info;
    }

    return file_info;
}

int save_file(const char* file_path, const char* data)
{
    FILE* fp = get_file_pointer(file_path);

    if (fp == NULL)
    {
        fprintf(stderr, "Failed to open or create file: %s\n", file_path);
        return -1;
    }

    const int write_result = fprintf(fp, "%s", data);

    // 書き込みが成功したか確認
    if (write_result < 0)
    {
        fprintf(stderr, "Error writing to file %s: %s\n", file_path,
                strerror(errno));

        const int close_result = fclose(fp);

        // クローズが成功したか確認
        if (close_result != 0)
        {
            fprintf(stderr, "Error closing file %s: %s\n", file_path,
                    strerror(errno));
            return -1;
        }

        return -1;
    }

    const int close_result = fclose(fp);

    // クローズが成功したか確認
    if (close_result != 0)
    {
        fprintf(stderr, "Error closing file %s: %s\n", file_path,
                strerror(errno));
        return -1;
    }

    return 0;
}
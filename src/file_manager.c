#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

//#include "config.h"
#include "display.h"
#include "file_manager.h"
//#include "input_handler.h"

#define LIMIT_LINE_LEN 10000

int read_file(const char *file_path);
// ファイルのパスを引数に指定するとファイルを読み込む関数
FILE *get_file_pointer(const char *file_path);
// ファイルのポインタを返す関数
int save_file(const char *file_path, const char *data);
// ファイルをセーブする関数

int read_file(const char *file_path)
{
    // ファイルのパスを引数に指定するとファイルを読み込む関数

    FILE *fp;
    char *line;
    // getline用のバッファ
    char *content[LIMIT_LINE_LEN];
    // とりえず10000行読み込める
    size_t len;
    ssize_t read;
    int count;
    int close_result;
    int line_count;

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

    close_result = fclose(fp);

    if (close_result != 0)
    {
        fprintf(stderr, "Error closing file %s: %s\n", file_path, strerror(errno));

        return -1;
    }

    line_count = 0;

    render_screen(content, count);

    return 0;
}

FILE *get_file_pointer(const char *file_path)
{
    // ファイルのポインタを返す関数

    FILE *file;

    file = fopen(file_path, "r+");
    // ファイルが存在して正常に開けた時の処理

    if (file != NULL)
    {
        return file;
    }

    file = fopen(file_path, "a+");
    // ファイルが存在して新しく作成する処理

    if (file != NULL)
    {
        return file;
    }

    printf("Error: %s", strerror(errno));

    return NULL;
}

int save_file(const char *file_path, const char *data)
{
    // ファイルをセーブする関数

    int write_result;
    int close_result;
    FILE *fp;

    fp = get_file_pointer(file_path);

    if (fp == NULL)
    {
        fprintf(stderr, "Failed to open or create file: %s\n", file_path);

        return -1;
    }

    write_result = fprintf(fp, "%s", data);

    if (write_result < 0)
    {
        // 書き込みが成功したか確認する条件式

        fprintf(stderr, "Error writing to file %s: %s\n", file_path, strerror(errno));

        close_result = fclose(fp);

        if (close_result != 0)
        {
            // クローズが成功したか確認する条件式

            fprintf(stderr, "Error closing file %s: %s\n", file_path, strerror(errno));

            return -1;
        }

        return -1;
    }

    close_result = fclose(fp);

    if (close_result != 0)
    {
        // クローズが成功したか確認する条件式

        fprintf(stderr, "Error closing file %s: %s\n", file_path, strerror(errno));

        return -1;
    }

    return 0;
}

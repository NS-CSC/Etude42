#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "file_manager.h"
#include "display.h"

#define LIMIT_LINE_LEN 10000

FILE *get_file_pointer(const char *file_path);
// ファイルのポインタを返す関数
int read_file(const char *file_path);
// ファイルのパスを引数に指定するとファイルを読み込む関数
int save_file(const char *file_path, const char *data);
// ファイルをセーブする関数

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

    int close_result

    close_result = fclose(fp);

    if (close_result != 0)
    {
        fprintf(stderr, "Error closing file %s: %s\n", file_path, strerror(errno));

        return -1;
    }

    // render_screen(content)
    // 描画については後にdisplay.cができてから
    // ↑最後の行数も合わせて呼び出したら普通に動作します

    return 0;
}

int save_file(const char *file_path, const char *data)
{
    // ファイルをセーブする関数

    FILE *fp;

    fp = get_file_pointer(file_path);

    if (fp == NULL)
    {
        fprintf(stderr, "Failed to open or create file: %s\n", file_path);

        return -1;
    }

    int write_result;

    write_result = fprintf(fp, "%s", data);

    if (write_result < 0)
    {
        // 書き込みが成功したか確認する条件式

        fprintf(stderr, "Error writing to file %s: %s\n", file_path, strerror(errno));

        int close_result;

        close_result =  fclose(fp);

        if (close_result != 0)
        {
            // クローズが成功したか確認する条件式

            fprintf(stderr, "Error closing file %s: %s\n", file_path, strerror(errno));

            return -1;
        }

        return -1;
    }

    int close_result;

    close_result = fclose(fp);

    if (close_result != 0)
    {
        // クローズが成功したか確認する条件式

        fprintf(stderr, "Error closing file %s: %s\n", file_path, strerror(errno));

        return -1;
    }

    return 0;
}

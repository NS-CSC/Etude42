#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

//#include "config.h"
#include "display.h"
#include "file_manager.h"
//#include "input_handler.h"

int read_file(const char *file_path);
// ファイルのパスを引数に指定するとファイルを読み込む関数
FILE *get_file_pointer(const char *file_path);
// ファイルのポインタを返す関数
int save_file(const char *file_path, const char *data);
// ファイルをセーブする関数
DIR *get_directory_pointer(const char *directory_path);
// 指定された一番上のディレクトリのポインタを返す関数

int read_file(const char *file_path)
{
    // ファイルのパスを引数に指定するとファイルを読み込む関数

    FILE *fp;
    int file_len;
    // ファイルの行数
    char *line;
    // 読み込みバッファ
    wchar_t *w_line;
    wchar_t **content;
    // ワイド文字リテラルの先頭のポインタの配列
    wchar_t **tmp;
    // reallocで使う一時的なポインタ
    int count;
    // ファイルの行数をカウントする変数
    int close_result;
    // ファイルをクローズする際の返り値
    int line_count;
    // ファイルの行数をカウントする変数 printfで使う
    int mbstows_result;
    // mbtowc関数の返り値
    size_t getline_len;
    // getline関数の返り値
    int free_index;
    // メモリを解放するためのインデックス
    getline_len = 0;
    file_len = 500;
    count = 0;

    fp = get_file_pointer(file_path);

    if (fp == NULL)
    {
        fprintf(stderr, "Failed to open or create file: %s\n", file_path);
        return -1;
    }

    content = (wchar_t **)malloc(sizeof(wchar_t *) * file_len);
    // ファイルの行数を指定してメモリを確保

    if (content == NULL)
    {
        fprintf(stderr, "Failed to allocate memory\n");
        free(content);
        return -1;
    }

    while (getline(&line, &getline_len, fp) != -1)
    {
        if (count == file_len)
        {
            // 行数を超えた場合の処理 メモリを再確保する
            file_len *= 2;
            tmp = (wchar_t **)realloc(content, sizeof(wchar_t *) * file_len);

            if (tmp == NULL)
            {
                fprintf(stderr, "Failed to allocate memory\n");
                free_index = 0;
                while (free_index < count)
                {
                    free(content[free_index]);
                    free_index++;
                }
                free(content);
                free(tmp);
                return -1;
            }

            content = tmp;
        }

        mbstows_result = mbstowcs(NULL, line, 0) + 1;
        w_line = (wchar_t *)malloc(sizeof(wchar_t) * mbstows_result);
        if (w_line == NULL)
        {
            fprintf(stderr, "Failed to allocate memory\n");
            free(w_line);
            return -1;
        }

        mbstowcs(w_line, line, mbstows_result);
        content[count] = w_line;
        count++;
    }

    close_result = fclose(fp);

    if (close_result != 0)
    {
        fprintf(stderr, "Error closing file %s: %s\n", file_path, strerror(errno));

        return -1;
    }

    render_screen(content, count);

    return 0;
}

DIR *get_directory_pointer(const char *directory_path)
{
    // 指定された一番上のディレクトリのポインタを返す関数

    DIR *dir; // DIR型ポインタ
    dir = opendir(directory_path);

    if (dir == NULL)
    {
        fprintf(stderr, "Error opening directory %s: %s\n", directory_path, strerror(errno));
        return NULL;
    }

    return dir;
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

/**
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>


void print_indents(int depth, int branch[])
{
    //階層を表しているdepthと縦線を引くかどうかを表すbranchを受け取って、適切なインデントを出力する関数 

    int i;
    i = 0;

    while (i < depth)
    {
        if (branch[i])
        {
            printf("│   ");
        } else
        {
            printf("    ");
        }
        i++;
    }
}

int directory_list(const char* directory_path, int depth, int branch[])
{
    //depthは階層を表している mainで呼ぶときは０を指定する branchは縦線を引くかどうかを表す配列

    DIR *dir;
    struct dirent *dp;
    char new_path[2049];
    // directory_pathとdp->d_nameを結合して新しいパスを作るための文字列 長さはそれぞれの最大値が1024, パスを結合するときに/が入るので1を足す 
    int i = 0;
    // ループ用の変数
    int entry_count = 0;
    //ディレクトリ内の要素の数
    int entry_index = 0;
    //ディレクトリ内の要素のインデックス

    dir = get_directory_pointer(directory_path);

    if (dir == NULL)
    {
        return -1;
    }
    if (depth == 0)
    {
        printf(".\n├──\n");
    }

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        //カレントディレクトリや親ディレクトリは子要素ではないのでカウントしない
        {
            entry_count++;
        }
    }

    rewinddir(dir);
    //ディレクトリのポインタを先頭に戻す

    while ((dp = readdir(dir)) != NULL)
    // これは指定されたディレクトリの一つ下の要素を順番に探っていく なくなるのとNULLが返ってくる
    {

        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
        // カレントディレクトリや親ディレクトリに行くと無限ループになるので回避する
        {
            continue;
        }
        
        int is_last = (entry_count - 1 == entry_index);
        // 最後の要素かどうかを判定する entry_indexは0から始まるので-1する

        print_indents(depth, branch);

        if (is_last)
        {
            printf("└── ");
        } else
        {
            printf("├── ");
        }

        printf("%s\n", dp->d_name);

        entry_index++;

        if (dp->d_type == DT_DIR)
        //dp->d_typeは要素がディレクトリかファイルかを返す DT_DIRの場合はディレクトリ DT_REGはファイル
        {
            snprintf(new_path, sizeof(new_path), "%s/%s", directory_path, dp->d_name);
            // 現在のディレクトリとその子要素のディレクトリのパスを結合
            branch[depth] = !is_last;
            //最後の呼び出しでないなら縦線を残す
            directory_list(new_path, depth + 1, branch);
            //階層を1つ深くして再帰的に呼び出す
        }

    }

    closedir(dir);
    
    return 0;
}

int main(void)
{
    int result;
    result = 0;

    int branch[100] = {0};
    result = directory_list("/mnt/d/workspace/etude42/", 0, branch);
    // pathは適宜変更する

    if (result != 0)
    {
        return -1;
    }

    return 0;
}

*/

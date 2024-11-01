#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


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




//書き込み権限があるか
int check_read_permission(const char* filename)
{
    if (access(filename, R_OK) == -1) 
    {
        if (errno == EACCES) 
        {
            printf(stderr, "ERROR: '%s' への読み込み権限がありません\n", filename);
            return -1;
        } else if (errno == ENOENT) {
            printf(stderr, "ERROR: ファイル '%s' が存在しません\n", filename);
            return -2;
        } else {
            printf(stderr, "ERROR: '%s' \n", strerror(errno));
            return -3;
        }
    }
    // 読み込み権限あり
    return 0; 
}

//書き込み権限があるか
int check_write_permission(const char* filename)
{
    if (access(filename, W_OK) == -1)
    {
        if (errno == EACCES) {
            printf(stderr, "ERROR:'%s'への書き込み権限がありません\n", filename);
            return -1;
        } else if (errno == ENOENT) {
            printf(stderr, "ERROR: ファイル'%s'が存在しません\n", filename);
            return -2;
        } else {
            printf(stderr, "ERROR: %s\n", strerror(errno));
            return -3;
        }
    }
    // 書き込み権限あり
    return 0; 
}

//読み書き権限
int print_permission(const char* filename)
{
    int read_parmission = check_read_permission(filename);
    int write_permission = check_write_permission(filename);
    int result = 0;
    if (read_parmission != 0)
    {
        //読み込みエラー
        result = 1;

    } 
    if (write_permission != 0){
        //書き込みエラー
        result = 2;

    }
    if (read_parmission != 0 && write_permission != 0){

        //読み書きエラー
        result = 3;

    } else {
        printf("permission_errorNothing");
    }   

    return result;    
}

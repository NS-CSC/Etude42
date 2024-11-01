#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

//書き込み権限があるか
int check_read_permission(const char* filename) 
{
    if (access(filename, R_OK) == -1) 
    {
        if (errno == EACCES) 
        {
            printf(stderr, "ERROR: '%s' への読み込み権限がありません\n", filename);
            return -1;
        }
        else if (errno == ENOENT) 
        {
            printf(stderr, "ERROR: ファイル '%s' が存在しません\n", filename);
            return -2;
        } 
        else 
        {
            printf(stderr, "ERROR: %s\n", strerror(errno));
            return -3;
        }
    }
    // 書き込み権限あり
    return 0; 
}

//書き込み権限があるか
int check_write_permission(const char* filename) 
{
    if (access(filename, W_OK) == -1) 
    {
        if (errno == EACCES) 
        {
            printf(stderr, "ERROR:'%s'への書き込み権限がありません\n", filename);
            return -1;
        } 
        else if (errno == ENOENT) 
        {
            printf(stderr, "ERROR: ファイル'%s'が存在しません\n", filename);
            return -2;
        } 
        else 
        {
            printf(stderr, "ERROR: %s\n", strerror(errno));
            return -3;
        }
    }
    // 書き込み権限あり
    return 0; 
}

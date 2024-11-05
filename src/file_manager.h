#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "config.h"
#include "display.h"
#include "file_manager.h"
#include "input_handler.h"

FILE *get_file_pointer(const char *file_path);
// ファイルのポインタを返す関数
int read_file(const char *file_path);
// ファイルのパスを引数に指定するとファイルを読み込む関数
int save_file(const char *file_path, const char *data);
// ファイルをセーブする関数

#endif // FILE_MANAGER_H

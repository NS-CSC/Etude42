#ifndef DISPLAY_H
#define DISPLAY_H

#include <ncurses.h>
#include <stdlib.h>
#include <wchar.h>

//#include "config.h"
//#include "file_manager.h"
#include "input_handler.h"

void render_screen(char *file_data[], const int current_max_lines);
// ファイルの中身をポインタ配列で渡すとそれを画面に表示する関数
void update_screen(char *file_data[], const int current_max_lines, const int current_scroll);
// ファイルの中身をポインタ配列で渡すとそれを画面に表示する関数
int get_digits(int number);
// 十進数の桁数を求める関数

#endif // DISPLAY_H

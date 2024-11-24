#ifndef DISPLAY_H
#define DISPLAY_H

#include <ncurses.h>
#include <stdlib.h>
#include <wchar.h>

//#include "config.h"
//#include "file_manager.h"
#include "input_handler.h"

void render_screen(wchar_t **file_data, const int current_max_lines);
// ファイルの中身をポインタ配列で渡すとそれを画面に表示する関数
void update_screen(char *file_data[], const int current_max_lines, const int current_scroll, const int window_x, const int window_y, const unsigned short axis_flag, char **display_data);
// ファイルの中身をポインタ配列で渡すとそれを画面に表示する関数
void init();
// ncursesの初期設定をまとめた関数
int get_digits(int number);
// 十進数の桁数を求める関数
int get_display_width(char *str);
// 画面上の表示幅を取得する関数
void get_display_data(char **str1, char **str2, int indent_offset, int window_y, int current_max_lines);

#endif // DISPLAY_H

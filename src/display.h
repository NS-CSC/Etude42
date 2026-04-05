#ifndef DISPLAY_H
#define DISPLAY_H

#include <ncurses.h>
#include <stdlib.h>
#include <wchar.h>

void render_screen(wchar_t **file_data, const int current_max_lines);
// ファイルの中身をポインタ配列で渡すとそれを画面に表示する関数
void update_screen(wchar_t **display_lines, int display_line_count, const int *display_orig, const int *display_offsets, int current_scroll, int window_y, int indent_offset, const int *line_in_block, wchar_t **file_data);
// 表示用の行データを画面に描画する関数
void init();
// ncursesの初期設定をまとめた関数
int get_digits(int number);
// 十進数の桁数を求める関数
int get_display_width(char *str);
// 画面上の表示幅を取得する関数
wchar_t **get_display_str(wchar_t **file_data, int str_max_index, int window_x, int *display_str_max_index, int **display_orig, int **display_offsets);
int wchar_string_display_width(wchar_t *str);

#endif // DISPLAY_H

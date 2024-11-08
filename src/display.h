#ifndef DISPLAY_H
#define DISPLAY_H

#include <ncurses.h>
#include <string.h>

#include "config.h"
#include "display.h"
#include "file_manager.h"
#include "input_handler.h"

void render_screen(char *file_data[], const int current_max_lines);
// ファイルの中身をポインタ配列で渡すとそれを画面に表示する関数
int get_digits(int number);
// 十進数の桁数を求める関数
void input_handler(const int indent_offset, char *file_data[], const int current_max_lines);
// 別の関数を参照すると手間がかかるため、一時的にショートカットとして使用する関数
void move_mouse(int *cursor_pos_x, int *cursor_pos_y, const int indent_offset, const int line_len, const unsigned short left_arrow_flag, char *file_data[]);
// 仮想的なマウスの位置を実際の位置に移動させる関数
int strlen_utf8(const char *str);
// マルチバイト文字を含めた文字列の長さを返す関数
int get_char_size(const char *str, int length);
// 表示上の文字列の長さを取得する関数

#endif // DISPLAY_H

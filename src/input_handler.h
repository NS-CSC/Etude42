#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <ncurses.h>
#include <stdlib.h>
#include <wchar.h>

//#include "config.h"
#include "display.h"
//#include "file_manager.h"

void input_handler(const int indent_offset, char *file_data[], const int current_max_lines);
// 別の関数を参照すると手間がかかるため、一時的にショートカットとして使用する関数
void move_mouse(int *cursor_pos_x, int *cursor_pos_y, const int indent_offset, const int line_len, const unsigned short left_arrow_flag, char *file_data[]);
// 仮想的なマウスの位置を実際の位置に移動させる関数
int strlen_utf8(const char *str);
// マルチバイト文字を含めた文字列の長さを返す関数
int get_char_size(char *str, const int length);
// 表示上の文字列の増加量を取得する関数

#endif // INPUT_HANDLER_H

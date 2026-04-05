#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <ncurses.h>
#include <stdlib.h>
#include <wchar.h>

void input_handler(int content_start_col, wchar_t **display_lines, int display_line_count, int window_x, int window_y, const int *display_orig, const int *display_offsets, const int *line_in_block, wchar_t **file_data);
// 入力を受け取り、スクロールとカーソルを制御する関数

#endif // INPUT_HANDLER_H

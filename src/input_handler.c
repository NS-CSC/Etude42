#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

//#include "config.h"
#include "display.h"
//#include "file_manager.h"
#include "input_handler.h"

void input_handler(int content_start_col, wchar_t **display_lines, int display_line_count, int window_x, int window_y, const int *display_orig, const int *display_offsets, const int *line_in_block, wchar_t **file_data);
// 入力を受け取り、スクロールとカーソルを制御する関数

static int wchar_visual_width(wchar_t ch);
static int string_visual_width(const wchar_t *str);
static int clamp_visual_col(const wchar_t *str, int desired_col);
static int move_right_col(const wchar_t *str, int current_col);
static int move_left_col(const wchar_t *str, int current_col);
static void ensure_visible_and_render(int cursor_line, int cursor_col, int *current_scroll, int display_line_count, int window_y, int content_start_col, wchar_t **display_lines, const int *display_orig, const int *display_offsets, int window_x, const int *line_in_block, wchar_t **file_data);

void input_handler(int content_start_col, wchar_t **display_lines, int display_line_count, int window_x, int window_y, const int *display_orig, const int *display_offsets, const int *line_in_block, wchar_t **file_data)
{
    int cursor_line;
    int cursor_col;
    int desired_col;
    int current_scroll;
    wchar_t *current_line;

    cursor_line = 0;
    cursor_col = 0;
    desired_col = 0;
    current_scroll = 0;

    ensure_visible_and_render(cursor_line, cursor_col, &current_scroll, display_line_count, window_y, content_start_col, display_lines, display_orig, display_offsets, window_x, line_in_block, file_data);

    while (1)
    {
        switch (getch())
        {
            case KEY_UP:
            case 'k':
                if (cursor_line > 0)
                {
                    cursor_line--;
                    current_line = display_lines[cursor_line];
                    cursor_col = clamp_visual_col(current_line, desired_col);
                }
                break;
            case KEY_DOWN:
            case 'j':
                if (cursor_line < display_line_count - 1)
                {
                    cursor_line++;
                    current_line = display_lines[cursor_line];
                    cursor_col = clamp_visual_col(current_line, desired_col);
                }
                break;
            case KEY_LEFT:
            case 'h':
                current_line = display_lines[cursor_line];
                cursor_col = move_left_col(current_line, cursor_col);
                desired_col = cursor_col;
                break;
            case KEY_RIGHT:
            case 'l':
                current_line = display_lines[cursor_line];
                cursor_col = move_right_col(current_line, cursor_col);
                desired_col = cursor_col;
                break;
            case 'q':
                return;
            default:
                break;
        }

        ensure_visible_and_render(cursor_line, cursor_col, &current_scroll, display_line_count, window_y, content_start_col, display_lines, display_orig, display_offsets, window_x, line_in_block, file_data);
    }
}
static int wchar_visual_width(wchar_t ch)
{
    int width;

    width = wcwidth(ch);

    if (width < 0)
    {
        return 0;
    }

    return width;
}

static int string_visual_width(const wchar_t *str)
{
    int i;
    int width_sum;

    i = 0;
    width_sum = 0;

    while (str[i] != L'\0')
    {
        width_sum += wchar_visual_width(str[i]);
        i++;
    }

    return width_sum;
}

static int clamp_visual_col(const wchar_t *str, int desired_col)
{
    int i;
    int width_sum;
    int next_width;

    i = 0;
    width_sum = 0;

    while (str[i] != L'\0')
    {
        next_width = width_sum + wchar_visual_width(str[i]);

        if (next_width > desired_col)
        {
            break;
        }

        width_sum = next_width;
        i++;
    }

    return width_sum;
}

static int move_right_col(const wchar_t *str, int current_col)
{
    int i;
    int width_sum;
    int next_width;
    int total_width;

    i = 0;
    width_sum = 0;
    total_width = string_visual_width(str);

    while (str[i] != L'\0')
    {
        next_width = width_sum + wchar_visual_width(str[i]);

        if (next_width > current_col)
        {
            return next_width;
        }

        width_sum = next_width;
        i++;
    }

    return total_width;
}

static int move_left_col(const wchar_t *str, int current_col)
{
    int i;
    int width_sum;
    int next_width;

    if (current_col <= 0)
    {
        return 0;
    }

    i = 0;
    width_sum = 0;

    while (str[i] != L'\0')
    {
        next_width = width_sum + wchar_visual_width(str[i]);

        if (next_width >= current_col)
        {
            return width_sum;
        }

        width_sum = next_width;
        i++;
    }

    return width_sum;
}

static void ensure_visible_and_render(int cursor_line, int cursor_col, int *current_scroll, int display_line_count, int window_y, int content_start_col, wchar_t **display_lines, const int *display_orig, const int *display_offsets, int window_x, const int *line_in_block, wchar_t **file_data)
{
    int screen_col;

    if (cursor_line < *current_scroll)
    {
        *current_scroll = cursor_line;
    }
    else if (cursor_line >= *current_scroll + window_y)
    {
        *current_scroll = cursor_line - window_y + 1;
    }

    update_screen(display_lines, display_line_count, display_orig, display_offsets, *current_scroll, window_y, content_start_col - 1, line_in_block, file_data);

    screen_col = content_start_col + cursor_col;
    if (screen_col >= window_x)
    {
        screen_col = window_x - 1;
    }

    move(cursor_line - *current_scroll, screen_col);
    refresh();
}

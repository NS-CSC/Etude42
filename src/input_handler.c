#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

//#include "config.h"
#include "display.h"
//#include "file_manager.h"
#include "input_handler.h"

void input_handler(const int indent_offset, char *file_data[], const int current_max_lines, int window_x, int window_y);
// 別の関数を参照すると手間がかかるため、一時的にショートカットとして使用する関数
void move_mouse(int *cursor_pos_x, int *cursor_pos_y, const int indent_offset, const int line_len, const unsigned short left_arrow_flag, char *file_data[], int *current_scroll, const int window_x, const int window_y, const int current_max_lines);
// 仮想的なマウスの位置を実際の位置に移動させる関数
int strlen_utf8(const char *str);
// マルチバイト文字を含めた文字列の長さを返す関数
int get_display_width_increment(char *str, const int length);
// 表示上の文字列の増加量を取得する関数

void input_handler(const int indent_offset, char *file_data[], const int current_max_lines, const int window_x, const int window_y)
{
    // 別の関数を参照すると手間がかかるため、一時的にショートカットとして使用する関数
    // 引数として行数インデントの数を持つ

    int cursor_pos_x;
    int cursor_pos_y;
    int current_scroll;

    cursor_pos_y = indent_offset;

    move(0, indent_offset);

    cursor_pos_x = 0;
    current_scroll = 0;

    while (1)
    {
        switch (getch())
        {
            case KEY_UP:
                if (cursor_pos_x > 0)
                {
                    cursor_pos_x--;
                    move_mouse(&cursor_pos_x, &cursor_pos_y, indent_offset, strlen_utf8(file_data[cursor_pos_x]) + indent_offset, 0, file_data, &current_scroll, window_x, window_y, current_max_lines);
                }

                break;
            case KEY_DOWN:
                if (cursor_pos_x < current_max_lines - 1)
                {
                    cursor_pos_x++;
                    move_mouse(&cursor_pos_x, &cursor_pos_y, indent_offset, strlen_utf8(file_data[cursor_pos_x]) + indent_offset, 0, file_data, &current_scroll, window_x, window_y, current_max_lines);
                }

                break;
            case KEY_LEFT:
                if (cursor_pos_y > indent_offset)
                {
                    cursor_pos_y--;
                    move_mouse(&cursor_pos_x, &cursor_pos_y, indent_offset, strlen_utf8(file_data[cursor_pos_x]) + indent_offset, 1, file_data, &current_scroll, window_x, window_y, current_max_lines);
                }

                break;
            case KEY_RIGHT:
                if (cursor_pos_y < strlen_utf8(file_data[cursor_pos_x]) + indent_offset - 2)
                {
                    cursor_pos_y++;
                    move_mouse(&cursor_pos_x, &cursor_pos_y, indent_offset, strlen_utf8(file_data[cursor_pos_x]) + indent_offset, 0, file_data, &current_scroll, window_x, window_y, current_max_lines);
                }

                break;
            case 'q':
                return;

                break;
                //case 'j':
                //    current_scroll++;
                //    // いつか頑張って判定をとる

                //    update_screen(file_data, current_max_lines, current_scroll, window_x, window_y);
                //    move_mouse(&cursor_pos_x, &cursor_pos_y, indent_offset, strlen_utf8(file_data[cursor_pos_x]) + indent_offset, 0, file_data, &current_scroll, window_x, window_y, current_max_lines);

                //    break;
                //case 'k':
                //    if (current_scroll > 0)
                //    {
                //        current_scroll--;
                //        update_screen(file_data, current_max_lines, current_scroll, window_x, window_y);
                //        move_mouse(&cursor_pos_x, &cursor_pos_y, indent_offset, strlen_utf8(file_data[cursor_pos_x]) + indent_offset, 0, file_data, &current_scroll, window_x, window_y, current_max_lines);
                //    }
                //
                //    break;
        }
    }

    return;
}

void move_mouse(int *cursor_pos_x, int *cursor_pos_y, const int indent_offset, const int line_len, const unsigned short left_arrow_flag, char *file_data[], int *current_scroll, const int window_x, const int window_y, const int current_max_lines)
{
    // 仮想的なマウスの位置を実際の位置に移動させる関数
    // 今は一マスの移動のみに対応している
    // 大きく移動させたいときは移動したい方向も引数で渡すことにする

    int i;
    int display_line_len;
    int x_offset;
    int index_data;
    int display_offset;
    int line;
    char **display_data;

    *display_data = malloc(10000);

    get_display_data(display_data, file_data, indent_offset, window_y, current_max_lines);

    index_data = 0;
    display_offset = 0;


    if (*cursor_pos_y > line_len - 2)
    {
        if (left_arrow_flag == 1)
        {
            if (line_len - 3 > indent_offset)
            {
                *cursor_pos_y = line_len - 3;
            }

            else
            {
                *cursor_pos_y = indent_offset;
            }
        }

        else
        {
            i = *current_scroll;
            x_offset = 0;

            while (*current_scroll + *cursor_pos_x > i)
            {
                display_line_len = get_display_width(file_data[i]) + indent_offset;

                while (display_line_len > window_y)
                {
                    display_line_len -= window_y;

                    x_offset++;
                }

                i++;
            }

            // 初期の表示最終行から-1して
            // オフセットを含めた最後の行を取得して、それからポインタの範囲をとる
            // if x_オフセットを含めた最後の行<=カーソルのx
            //     while 描画範囲内の一番上のx(x_オフセットより小さくはならない)<カーソルのx
            //         それぞれの行のオフセットを取得し、総和を代入する

            // ↑行合わせを上と下に変更する
            // 上合わせと下合わせをunsigned short intで管理する
            // 下合わせは0で上合わせは1

            if (window_x + *current_scroll - x_offset <= *cursor_pos_x)
            {
                // カーソルが描画範囲より下に出ていたときの条件式

                current_scroll = window_x * *current_scroll - x_offset + current_scroll;
                //(*current_scroll)++;
                // とりあえず+1
                update_screen(file_data, current_max_lines, *current_scroll, window_x, window_y, 0, display_data);
            }

            else if (*current_scroll >= *cursor_pos_x)
            {
                // カーソルが描画範囲より上に出ていたときの条件式

                (*current_scroll)--;
                // とりあえず-1
                update_screen(file_data, current_max_lines, *current_scroll, window_x, window_y, 1, display_data);
            }

            move(*cursor_pos_x - *current_scroll + x_offset, line_len - 1 + get_display_width_increment(file_data[*cursor_pos_x], *cursor_pos_y - indent_offset));
            // 上下に移動した時、位置が合わない問題が発生するので、これに対処する必要がある。

            return;
        }
    }

    i = *current_scroll;
    x_offset = 0;

    while (*current_scroll + *cursor_pos_x > i)
    {
        display_line_len = get_display_width(file_data[i]) + indent_offset;

        while (display_line_len > window_y)
        {
            display_line_len -= window_y;

            x_offset++;
        }

        i++;
    }

    if (window_x + *current_scroll - x_offset <= *cursor_pos_x)
    {
        // カーソルが描画範囲より下に出ていたときの条件式

        //current_scroll = window_x * *current_scroll - x_offset + current_scroll;
        (*current_scroll)++;
        // とりあえず+1
        update_screen(file_data, current_max_lines, *current_scroll, window_x, window_y, 0, display_data);
    }

    else if (*current_scroll > *cursor_pos_x)
    {
        // カーソルが描画範囲より上に出ていたときの条件式

        (*current_scroll)--;
        // とりあえず-1
        update_screen(file_data, current_max_lines, *current_scroll, window_x, window_y, 1, display_data);
    }

    move(*cursor_pos_x - *current_scroll + x_offset, *cursor_pos_y + get_display_width_increment(file_data[*cursor_pos_x], *cursor_pos_y - indent_offset));
    // 上下に移動した時、位置が合わない問題が発生するので、これに対処する必要がある。

    return;
}

int strlen_utf8(const char *str)
{
    // マルチバイト文字を含めた文字列の長さを返す関数

    int str_index;
    int number;

    str_index = 0;
    number = 0;

    while (str[str_index])
    {
        if ((str[str_index] & 0xc0) != 0x80)
        {
            number++;
        }

        str_index++;
    }

    return number + 1;
}

int get_display_width_increment(char *str, const int length)
{
    // 表示上の文字列の増加量を取得する関数

    int width_sum;
    wchar_t wc;
    int bytes_read;
    int chars_processed;
    char *current_ptr;
    int width;

    width_sum = 0;
    bytes_read = 0;
    chars_processed = 0;
    current_ptr = str;

    while (chars_processed < length && *current_ptr != '\0')
    {
        // 指定された文字数(length)まで処理する

        bytes_read = mbtowc(&wc, current_ptr, MB_CUR_MAX);

        if (bytes_read <= 0)
        {
            puts("マルチバイト文字の変換に失敗しました。");

            return -1;
        }

        width = wcwidth(wc);

        if (width >= 0)
        {
            width_sum += width;
        }

        current_ptr += bytes_read;

        chars_processed++;
    }

    // 半角との差分を返すために width_sum - chars_processed を返す
    return width_sum - chars_processed;
}

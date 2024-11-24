#include <ncurses.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

//#include "config.h"
#include "display.h"
//#include "file_manager.h"
#include "input_handler.h"

void render_screen(char *file_data[], const int current_max_lines);
// ファイルの中身をポインタ配列で渡すとそれを画面に表示する関数
void update_screen(char *file_data[], const int current_max_lines, const int current_scroll, const int window_x, const int window_y, const unsigned short axis_flag, char **display_data);
// ファイルの中身をポインタ配列で渡すとそれを画面に表示する関数
int get_digits(int number);
// 十進数の桁数を求める関数
int get_display_width(char *str);
// 画面上の表示幅を取得する関数
void get_display_data(char **display_data, char **file_data, int indent_offset, int window_y, int current_max_lines);

void render_screen(char *file_data[], const int current_max_lines)
{
    // ファイルの中身をポインタ配列で渡すとそれを画面に表示する関数
    // 最初に呼び出す関数ともう一度書き直すものに分割する
    // ディスプレイサイズによる描画の範囲（下限）が書いていないため、対応させる必要がある。

    int number;
    int indent_space;
    int x_offset;
    int line_len;
    int window_x;
    int window_y;
    char **display_data;

    initscr();
    noecho();
    keypad(stdscr, TRUE);
    getmaxyx(stdscr, window_x, window_y);
    // ncursesの初期設定
    
    *display_data = malloc(10000);

    get_display_data(display_data, file_data, indent_space + 1, window_y, current_max_lines);

    endwin();
    for (int i = 0; i < 60; i++)
    {
        puts(display_data[i]);
    }
    exit(0);

    number = 0;
    indent_space = get_digits(current_max_lines);
    x_offset = 0;

    while (number < window_x)
    {
        //mvprintw(number + x_offset, 0, "%*d %s", indent_space, number + 1, file_data[number]);
        mvprintw(number, 0, "%*d %s", indent_space, number + 1, display_data[number]);

        //line_len = get_display_width(file_data[number]) + indent_space + 1;

        //endwin();
        //puts(display_data[0]);
        //exit(0);

        //while (line_len > window_y)
        //{
        //    line_len -= window_y;

        //    x_offset++;
        //}

        number++;
    }

    refresh();

    input_handler(indent_space + 1, file_data, current_max_lines, window_x, window_y);

    endwin();
    // 描画を終了する

    return;
}

void update_screen(char *file_data[], const int current_max_lines, const int current_scroll, const int window_x, const int window_y, const unsigned short axis_flag, char **display_data)
{
    // ファイルの中身をポインタ配列で渡すとそれを画面に表示する関数

    int number;
    int indent_space;
    int x_offset;
    int line_len;

    erase();
    // 描画した画面を削除

    number = 0;
    indent_space = get_digits(current_max_lines);
    x_offset = 0;

    while (number + current_scroll < current_max_lines)
    {
        //mvprintw(number + x_offset, 0, "%*d %s", indent_space, number + 1 + current_scroll, file_data[number + current_scroll]);
        mvprintw(number, 0, "%*d %s", indent_space, number + 1 + current_scroll, display_data[number]);
        // x_display_offsetはまだバグがあるので、ここを修正する。

        line_len = get_display_width(file_data[number + current_scroll]) + indent_space + 1;

        while (line_len > window_y)
        {
            line_len -= window_y;

            x_offset++;
        }

        number++;
    }

    refresh();

    return;
}

int get_digits(int number)
{
    // 十進数の桁数を求める関数

    int answer;

    answer = 1;

    while (number >= 10)
    {
        number /= 10;

        answer++;
    }

    return answer;
}

int get_display_width(char *str)
{
    // 画面上の表示幅を取得する関数

    int len;
    wchar_t *wstr;
    int test;
    int conversion_result;
    int i;
    int char_width;

    len = mbstowcs(NULL, str, 0);

    if (len == -1)
    {
        puts("char*をwchar_t*に変換する段階で問題が発生しました。");

        exit(1);
    }

    wstr = malloc((len + 1) * sizeof(wchar_t));

    if (wstr == NULL)
    {
        puts("mallocに失敗しました。");

        exit(1);
    }

    test = mbstowcs(wstr, str, len + 1);

    if (test == -1)
    {
        puts("char*をwchar_t*に変換する段階で問題が発生しました。");

        free(wstr);

        exit(1);
    }

    i = 0;
    conversion_result = 0;

    while (i < len)
    {
        char_width = wcwidth(wstr[i]);

        if (char_width == -1)
        {
            char_width = 0;
        }

        conversion_result += char_width;

        i++;
    }

    free(wstr);

    return conversion_result;
}

void get_display_data(char **display_data, char **file_data, int indent_offset, int window_y, int current_max_lines)
{
    int index_data;
    int display_offset;
    int display_offset1;
    int line;
    int max_length;
    int i;

    index_data = 0;
    display_offset = 0;
    line = 0;

    while (index_data < current_max_lines)
    {
        display_offset1 = 0;

        //if (index_data == 1)
        //{
        //    endwin();
        //    puts(file_data[index_data + 1]);
        //    break;
        //}

        line = get_display_width(file_data[index_data]);

        //if (index_data == 2) break;

        if (line <= window_y - indent_offset)
        {
            //strcpy(display_data[index_data + display_offset], file_data[index_data]);
            display_data[index_data + display_offset] = file_data[index_data];

            index_data++;

            continue;
        }

        do
        {
            max_length = window_y - indent_offset;

            display_data[index_data + display_offset] = (char *)malloc(max_length + 1);

            if (display_data[index_data + display_offset] == 0)
            {
                exit(1);
            }

            i = 0;

            while (i < max_length)
            {
                display_data[index_data + display_offset][i] = file_data[index_data][(window_y - indent_offset) * display_offset1 + i];

                i++;
            }

            display_data[index_data + display_offset][max_length] = '\0';

            line -= max_length;

            display_offset++;
            display_offset1++;
        } while (line > window_y - indent_offset);

        index_data++;
    }

    return;
}

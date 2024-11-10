#include <ncurses.h>
#include <stdlib.h>

//#include "config.h"
#include "display.h"
//#include "file_manager.h"
#include "input_handler.h"

void render_screen(char *file_data[], const int current_max_lines);
// ファイルの中身をポインタ配列で渡すとそれを画面に表示する関数
void update_screen(char *file_data[], const int current_max_lines, const int current_scroll);
// ファイルの中身をポインタ配列で渡すとそれを画面に表示する関数
int get_digits(int number);
// 十進数の桁数を求める関数

void render_screen(char *file_data[], const int current_max_lines)
{
    // ファイルの中身をポインタ配列で渡すとそれを画面に表示する関数
    // 最初に呼び出す関数ともう一度書き直すものに分割する
    // ディスプレイサイズによる描画の範囲（下限）が書いていないため、対応させる必要がある。

    int number;
    int indent_space;

    initscr();
    noecho();
    keypad(stdscr, TRUE);
    // ncursesの初期設定

    number = 0;
    indent_space = get_digits(current_max_lines);

    while (file_data[number] != NULL)
    {
        mvprintw(number, 0, "%*d %s", indent_space, number + 1, file_data[number]);

        number++;
    }

    refresh();

    input_handler(indent_space + 1, file_data, current_max_lines);

    endwin();
    // 描画を終了する

    return;
}

void update_screen(char *file_data[], const int current_max_lines, const int current_scroll)
{
    // ファイルの中身をポインタ配列で渡すとそれを画面に表示する関数

    int number;
    int indent_space;

    erase();
    // 描画した画面を削除

    number = 0;
    indent_space = get_digits(current_max_lines);

    while (number + current_scroll < current_max_lines)
    {
        // ここの条件式をNULLで取るとバグるので注意が必要

        mvprintw(number, 0, "%*d %s", indent_space, number + 1 + current_scroll, file_data[number + current_scroll]);

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

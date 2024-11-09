#include <ncurses.h>
#include <stdlib.h>

//#include "config.h"
#include "display.h"
//#include "file_manager.h"
#include "input_handler.h"

void render_screen(char *file_data[], const int current_max_lines);
// ファイルの中身をポインタ配列で渡すとそれを画面に表示する関数
int get_digits(int number);
// 十進数の桁数を求める関数

void render_screen(char *file_data[], const int current_max_lines)
{
    // ファイルの中身をポインタ配列で渡すとそれを画面に表示する関数
    // 関数の中身を分割して別々に呼んでもらう形に変更する

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

    input_handler(indent_space + 1, file_data, current_max_lines);

    endwin();

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


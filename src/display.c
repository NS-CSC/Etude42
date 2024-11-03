#include <ncurses.h>

// #include "file_manager.h"
// #include "display.h"
// #include "input_handler.h"
// #include "config.h"
//  必要に応じて変更する

void render_screen(char *file_data[], int current_max_lines);
int get_digits(int number);
void input_handler(void);

int main(void)
{
    // テストで使用するmain関数

    char *file_data[10];

    file_data[0] = "#include <stdio.h>";
    file_data[1] = "";
    file_data[2] = "int main(void)";
    file_data[3] = "{";
    file_data[4] = "    puts(\"Hello\");";
    file_data[5] = "    return 0;";
    file_data[6] = "}";
    file_data[7] = "";
    file_data[8] = NULL;

    render_screen(file_data, 8);

    return 0;
}

void render_screen(char *file_data[], int current_max_lines)
{
    // 画面を描画する関数

    int number;
    int indent_space;

    number = 0;
    indent_space = get_digits(current_max_lines);

    initscr();
    noecho();
    keypad(stdscr, TRUE);

    while (file_data[number] != NULL)
    {
        mvprintw(number, 0, "%*d %s", indent_space, number + 1, file_data[number]);
        number++;
    }

    input_handler();
 
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

    printf("桁数は%dです\n", answer);

    return answer;
}

void input_handler(void)
{
    // 臨時の入力処理を行う関数
    // 引数として行数インデントの数を持つ
    int mouse_position_x;
    int mouse_position_y;

    mouse_position_x = 0;
    mouse_position_y = 0;

    move(mouse_position_x, mouse_position_y);

    while (1)
    {
        switch (getch())
        {
            case KEY_UP:
                mouse_position_x--;
                move(mouse_position_x, mouse_position_y);
                // 移動させる関数はほしい。仮想的な座標と実際の座標は違うので、マウスのxy位置（ポインタ）と文字列の長さがあるとよい
                break;
            case KEY_DOWN:
                mouse_position_x++;
                move(mouse_position_x, mouse_position_y);
                break;
            case KEY_LEFT:
                mouse_position_y--;
                move(mouse_position_x, mouse_position_y);
                break;
            case KEY_RIGHT:
                mouse_position_y++;
                move(mouse_position_x, mouse_position_y);
                break;
            case 'q':
                return;
                break;
        }
    }
 }

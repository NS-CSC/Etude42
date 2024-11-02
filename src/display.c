#include <ncurses.h>

// #include "file_manager.h"
// #include "display.h"
// #include "input_handler.h"
// #include "config.h"
//  必要に応じて変更する

void render_screen(char *file_data[]);

//int main(void)
//{
//    // テストで使用するmain関数
//
//    char *file_data[10001];
//
//    file_data[0] = "#include <stdio.h>";
//    file_data[1] = "";
//    file_data[2] = "int main(void)";
//    file_data[3] = "{";
//    file_data[4] = "    puts(\"Hello\");";
//    file_data[5] = "    return 0;";
//    file_data[6] = "}";
//
//    render_screen(file_data);
//
//    return 0;
//}

void render_screen(char *file_data[])
{
    // 画面を描画する関数
    // もう一つファイルの桁数の情報が欲しい

    int number;

    int indent_space;
    
    number = 0;

    indent_space = 0;
    // この変数でコードの始まる位置を統一したいけれど、そもそも行数がわからないのでどうしようもない

    initscr();

    while (file_data[number] != NULL)
    {
        mvprintw(number, 0, "%4d %s", number + 1, file_data[number]);
        // %5dの長さを可変にしたいけれど、行数がわからないので対応不可
        number++;
    }
    
    getch();
    endwin();

    return;
}


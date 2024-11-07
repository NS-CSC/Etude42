#include <locale.h>
#include <ncurses.h>
#include <stdlib.h>
#include <wchar.h>

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
void move_mouse(int *cursor_pos_x, int *cursor_pos_y, const int indent_offset, const int line_len, const unsigned short left_arrow_flag);
// 仮想的なマウスの位置を実際の位置に移動させる関数
int strlen_utf8(const char *str);
// マルチバイト文字を含めた文字列の長さを返す関数

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

    setlocale(LC_ALL, "ja_JP.UTF-8");

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

void input_handler(const int indent_offset, char *file_data[], const int current_max_lines)
{
    // 別の関数を参照すると手間がかかるため、一時的にショートカットとして使用する関数
    // 引数として行数インデントの数を持つ

    int cursor_pos_x;
    int cursor_pos_y;

    cursor_pos_x = 0;
    cursor_pos_y = indent_offset;

    move(0, indent_offset);

    while (1)
    {
        switch (getch())
        {
            case KEY_UP:
                if (cursor_pos_x > 0)
                {
                    cursor_pos_x--;
                    move_mouse(&cursor_pos_x, &cursor_pos_y, indent_offset, strlen_utf8(file_data[cursor_pos_x]) + indent_offset, 0);
                }
                break;
            case KEY_DOWN:
                if (cursor_pos_x < current_max_lines - 1)
                {
                    cursor_pos_x++;
                    move_mouse(&cursor_pos_x, &cursor_pos_y, indent_offset, strlen_utf8(file_data[cursor_pos_x]) + indent_offset, 0);
                }
                break;
            case KEY_LEFT:
                if (cursor_pos_y > indent_offset)
                {
                    cursor_pos_y--;
                    move_mouse(&cursor_pos_x, &cursor_pos_y, indent_offset, strlen_utf8(file_data[cursor_pos_x]) + indent_offset, 1);
                }
                break;
            case KEY_RIGHT:
                if (cursor_pos_y < strlen_utf8(file_data[cursor_pos_x]) + indent_offset - 2)
                {
                    cursor_pos_y++;
                    move_mouse(&cursor_pos_x, &cursor_pos_y, indent_offset, strlen_utf8(file_data[cursor_pos_x]) + indent_offset, 0);
                }
                break;
            case 'q':
                return;
                break;
        }
    }

    return;
}

void move_mouse(int *cursor_pos_x, int *cursor_pos_y, const int indent_offset, const int line_len, const unsigned short left_arrow_flag)
{
    // 仮想的なマウスの位置を実際の位置に移動させる関数
    // これ他にも引数必要では?
    // 今は一マスの移動のみに対応している
    // 大きく移動させたいときは移動したい方向を引数で渡すことにする

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
            move(*cursor_pos_x, line_len - 2);

            return;
        }
    }

    move(*cursor_pos_x, *cursor_pos_y);

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

int get_char_size(const char *str)
{
   size_t len;
   wchar_t *result;
   int incriment;
   int number;
   int back;

   len = mbstowcs(NULL, str, 0);

   if (len == -1)
   {
      puts("マルチバイト文字を含めた文字数の長さを返す処理に失敗しました。");

      return -1;
   }

   result = malloc(len * sizeof(wchar_t));

   if (!result)
   {
      puts("mallocに失敗しました。");

      return -1;
   }

   mbstowcs(result, str, len);

   number = 0;

   while (incriment < result[number])
   {
      incriment = result[number];

      number++;
   }

   return back;
}

// int main(void)
// {
//     // テストで使用するmain関数
//
//     char *file_data[100];
//
//     file_data[0] = "#include <stdio.h>";
//     file_data[1] = "";
//     file_data[2] = "int main(void)";
//     file_data[3] = "{";
//     file_data[4] = "    char *hoge_hoge[10];";
//     file_data[5] = "    int number;";
//     file_data[6] = "";
//     file_data[7] = "    hoge_hoge[0] = \"#include <stdio.h>\";";
//     file_data[8] = "    hoge_hoge[1] = \"\";";
//     file_data[9] = "    hoge_hoge[2] = \"int main(void)\";";
//     file_data[10] = "    hoge_hoge[3] = \"{\";";
//     file_data[11] = "    hoge_hoge[4] = \"    puts(\"Hello\");\";";
//     file_data[12] = "    hoge_hoge[5] = \"    return 0;\";";
//     file_data[13] = "    hoge_hoge[6] = \"}\";";
//     file_data[14] = "    hoge_hoge[7] = NULL;";
//     file_data[15] = "";
//     file_data[16] = "    number = 0;";
//     file_data[17] = "";
//     file_data[18] = "    while (hoge_hoge[number] != NULL)";
//     file_data[19] = "    {";
//     file_data[20] = "        puts(hoge_hoge[number]);";
//     file_data[21] = "        number++;";
//     file_data[22] = "    }";
//     file_data[23] = "    return 0;";
//     file_data[24] = "}";
//     file_data[25] = "// これはC言語初心者の脳を破壊するコードです。";
//     file_data[26] = "// C言語初心者の皆様は、このコードをしっかり理解することで脳を破壊しましょう！私からのお願いです！";
//     file_data[27] = "A man visiting a graveyard saw a tombstone that read:";
//     file_data[28] = "“Here lies John Kelly, a lawyer and an honest man.”";
//     file_data[29] = "“How about that!” he exclaimed.";
//     file_data[30] = "“They’ve got three people buried in one grave.”";
//
//     file_data[31] = NULL;
//
//     render_screen(file_data, 31);
//
//     return 0;
// }

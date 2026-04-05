#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

#define BACK_GROUND 0

#define COMMENT 1
#define CHAR 2
#define MACRO 3
#define VARIABLE 4
#define CONTROL 5
#define FUNCTION 6
#define NUMBER 7
#define WHITE 8

void init();
int color_check(char *str, int *color, int word_flag, int number_flag);
int variable_check(char character);
int number_check(char character);
void finish_check(char *str, int *flag);
int convert_255(int value);

struct words
{
    char *data[100];
};

int main(void)
{
    int index;
    int number;
    char **str;
    int length;
    int color;
    int word_flag;
    int number_flag;

    init();

    str = calloc(100, sizeof(*str));

    if (str == NULL)
    {
        return 1;
    }

    str[0] = "#include <stdio.h>";
    str[1] = "#include <stdlib.h>";
    str[2] = "#include <string.h>";
    str[3] = "";
    str[4] = "int main(void)";
    str[5] = "{";
    str[6] = "    // 文字列を動的メモリ確保";
    str[7] = "    char *message = (char *)malloc(50 * sizeof(char));";
    str[8] = "";
    str[9] = "    if (message == NULL)";
    str[10] = "    {";
    str[11] = "        fprintf(stderr, \"メモリ割り当てに失敗しました\\n\");";
    str[12] = "";
    str[13] = "        return 1;";
    str[14] = "    }";
    str[15] = "";
    str[16] = "    strcpy(message, \"Hello, World\");";
    str[17] = "    printf(\"メッセージ: %s\\n\", message);";
    str[18] = "";
    str[19] = "    // 整数変数と簡単な計算";
    str[20] = "    int count = 0;";
    str[21] = "";
    str[22] = "    for (int i = 0; i < 10; i++)";
    str[23] = "    {";
    str[24] = "        count += i;";
    str[25] = "    }";
    str[26] = "";
    str[27] = "    printf(\"カウントの合計: %d\\n\", count);";
    str[28] = "";
    str[29] = "    // ファイル操作";
    str[30] = "    FILE *file = fopen(\"sampple.txt\", \"w\");";
    str[31] = "";
    str[32] = "    if (file == NULL)";
    str[33] = "    {";
    str[34] = "        perror(\"ファイルを開けませんでした\");";
    str[35] = "        free(message);";
    str[36] = "";
    str[37] = "        return 1;";
    str[38] = "    }";
    str[39] = "";
    str[40] = "    fprintf(file, \"ファイルに書き込み中: %s\\n\", message);";
    str[41] = "    fclose(file);";
    str[42] = "";
    str[43] = "    // 動的メモリ解放";
    str[44] = "    free(message);";
    str[45] = "";
    str[46] = "    // 文字列操作";
    str[47] = "    char str1[20] = \"Hello\";";
    str[48] = "    char str2[20] = \"World\";";
    str[49] = "    strcat(str1, \", \");";
    str[50] = "    strcat(str1, str2);";
    str[51] = "    printf(\"結合された文字列: %s\\n\", str1);";
    str[52] = "";
    str[53] = "    // 終了";
    str[54] = "    return 0;";
    str[55] = "}";

    str[56] = NULL;

    index = 0;
    length = 0;

    while (str[index] != NULL)
    {
        move(index + 1, 3);

        number = 0;

        if (length >= -2)
        {
            length = 0;
        }

        word_flag = 1;
        number_flag = 1;

        while (str[index][number] != '\0')
        {
            if (length < -1)
            {
                finish_check(str[index] + number, &length);
            }

            if (length == 0)
            {
                if (number > 0)
                {
                    if (variable_check(str[index][number - 1]) == 0)
                    {
                        word_flag = 1;
                    }

                    if (number_check(str[index][number]) == 0)
                    {
                        if (!(str[index][number] >= '0' && str[index][number] <= '9'))
                        {
                            number_flag = 1;
                        }
                    }

                    else
                    {
                        if (!(str[index][number] >= '0' && str[index][number] <= '9'))
                        {
                            number_flag = 0;
                        }
                    }
                }

                length = color_check(str[index] + number, &color, word_flag, number_flag);

                attron(COLOR_PAIR(color));
            }

            printw("%c", str[index][number]);

            if (length > 0)
            {
                length--;
            }

            number++;

            if (length == 0)
            {
                attroff(COLOR_PAIR(color));
            }
        }

        index++;
    }

    refresh();
    getch();

    endwin();

    free(str);

    return 0;
}

void init()
{
    setlocale(LC_CTYPE, "ja_JP.UTF-8");

    initscr();
    start_color();

    init_color(BACK_GROUND, convert_255(34), convert_255(36), convert_255(54));

    init_color(COMMENT, convert_255(97), convert_255(107), convert_255(163));
    init_color(CHAR, convert_255(186), convert_255(221), convert_255(135));
    init_color(MACRO, convert_255(133), convert_255(225), convert_255(252));
    init_color(VARIABLE, convert_255(102), convert_255(188), convert_255(255));
    init_color(CONTROL, convert_255(192), convert_255(154), convert_255(254));
    init_color(FUNCTION, convert_255(130), convert_255(170), convert_255(255));
    init_color(NUMBER, convert_255(255), convert_255(150), convert_255(108));
    init_color(WHITE, convert_255(200), convert_255(211), convert_255(245));

    init_pair(1, COMMENT, BACK_GROUND);
    init_pair(2, CHAR, BACK_GROUND);
    init_pair(3, MACRO, BACK_GROUND);
    init_pair(4, VARIABLE, BACK_GROUND);
    init_pair(5, CONTROL, BACK_GROUND);
    init_pair(6, FUNCTION, BACK_GROUND);
    init_pair(7, NUMBER, BACK_GROUND);
    init_pair(8, WHITE, BACK_GROUND);

    return;
}

int color_check(char *str, int *color, int word_flag, int number_flag)
{
    struct words macro =
    {
        .data =
        {
            "__DATE__",
            "__FILE__",
            "__LINE__",
            "__STDC__",
            "__STDC_HOSTED__",
            "__STDC_VERSION__",
            "__TIME__",
            "sizeof",

            "_IOFBF",
            "_IOLBF",
            "_IONBF",
            "BUFSIZ",
            "EOF",
            "FILENAME_MAX",
            "FOPEN_MAX",
            "L_tmpnam",
            "NULL",
            "SEEK_CUR",
            "SEEK_CUR",
            "WSEEK_END",
            "SEEK_END",
            "SEEK_SET",
            "SEEK_SET",
            "stderr",
            "stdin",
            "stdout",

            "EXIT_FAILURE",
            "EXIT_SUCCESS",
            "MB_CUR_MAX",
            "NULL",
            "RAND_MAX"
        }
    };

    struct words variable =
    {
        .data =
        {
            "auto",
            "char",
            "const",
            "double",
            "enum",
            "extern",
            "float",
            "int",
            "long",
            "register",
            "short",
            "signed",
            "static",
            "struct",
            "typedef",
            "union",
            "unsigned",
            "void",
            "volatile",

            "fpos_t",
            "size_t",
            "FILE",

            "div_t",
            "ldiv_t",
            //"size_t",
            "wchar_t"
        }
    };

    struct words control =
    {
        .data =
        {
            "break",
            "case",
            "continue",
            "default",
            "do",
            "else",
            "for",
            "goto",
            "if",
            "return",
            "switch",
            "while"
        }
    };

    struct words function =
    {
        .data =
        {
            "main",

            "clearerr",
            "fclose",
            "feof",
            "ferror",
            "fflush",
            "fgetc",
            "fgetpos",
            "fgets",
            "fopen",
            "fprintf",
            "fputc",
            "fputs",
            "fread",
            "freopen",
            "fscanf",
            "fseek",
            "fsetpos",
            "ftell",
            "fwrite",
            "getc",
            "getchar",
            "gets",
            "perror",
            "printf",
            "putc",
            "putchar",
            "puts",
            "remove",
            "rename",
            "rewind",
            "scanf",
            "setbuf",
            "setvbuf",
            "sprintf",
            "sscanf",
            "tmpfile",
            "tmpnam",
            "ungetc",
            "vfprintf",
            "vprintf",
            "vsprintf",

            "abort",
            "abs",
            "atexit",
            "atof",
            "atoi",
            "atol",
            "bsearch",
            "calloc",
            "div",
            "exit",
            "free",
            "getenv",
            "labs",
            "ldiv",
            "malloc",
            "mblen",
            "mbstowcs",
            "mbtowc",
            "qsort",
            "rand",
            "realloc",
            "srand",
            "strtod",
            "strtol",
            "strtoul",
            "system",
            "wcstombs",
            "wctomb",

            "memchr",
            "memcmp",
            "memcpy",
            "memmove",
            "memset",
            "strcat",
            "strchr",
            "strcmp",
            "strcoll",
            "strcpy",
            "strcspn",
            "strerror",
            "strlen",
            "strncat",
            "strncmp",
            "strncpy",
            "strpbrk",
            "strrchr",
            "strspn",
            "strstr",
            "strtok",
            "strxfrm",
        }
    };

    int number;

    {
        if (strncmp(str, "/*", strlen("/*")) == 0)
        {
            *color = 1;

            return -4;
        }

        if (strncmp(str, "//", strlen("//")) == 0)
        {
            *color = 1;

            return -1;
        }
    }

    if (strncmp(str, "#", strlen("#")) == 0)
    {
        *color = 3;

        return strlen(str);
    }

    {
        if (str[0] =='\'')
        {
            *color = 2;

            return -2;
        }

        if (str[0] =='\"')
        {
            *color = 2;

            return -3;
        }
    }


    number = 0;

    if (word_flag == 1)
    {
        while (macro.data[number] != NULL)
        {
            if (strncmp(str, macro.data[number], strlen(macro.data[number])) == 0)
            {
                *color = 3;

                return strlen(macro.data[number]);
            }

            number++;
        }

        number = 0;

        while (variable.data[number] != NULL)
        {
            if (strncmp(str, variable.data[number], strlen(variable.data[number])) == 0)
            {
                *color = 4;

                return strlen(variable.data[number]);
            }

            number++;
        }

        number = 0;

        while (control.data[number] != NULL)
        {
            if (strncmp(str, control.data[number], strlen(control.data[number])) == 0)
            {
                *color = 5;

                return strlen(control.data[number]);
            }

            number++;
        }

        number = 0;

        while (function.data[number] != NULL)
        {
            if (strncmp(str, function.data[number], strlen(function.data[number])) == 0)
            {
                *color = 6;

                return strlen(function.data[number]);
            }

            number++;
        }
    }

    if (number_flag == 1 && str[0] >= '0' && str[0] <= '9')
    {
        *color = 7;

        return 1;
    }

    if (str[0] == '<' || str[0] == '>' || str[0] == '+' || str[0] == '-' || str[0] == '=' || str[0] == '*' || str[0] == '/' || str[0] == '&' || str[0] == '%' || str[0] == '!' || str[0] == '^' || str[0] == '~')
    {
        *color = 3;

        return 1;
    }

    *color = 8;

    return 1;
}

int variable_check(char character)
{
    if (character >= 'a' && character <= 'z' || character >= 'A' && character <= 'Z' || character >= '0' && character <= '9' || character == '_')
    {
        return 1;
    }

    return 0;
}

int number_check(char character)
{
    if (character >= 'a' && character <= 'z' || character >= 'A' && character <= 'Z' || character == '_')
    {
        return 1;
    }

    return 0;
}

void finish_check(char *str, int *flag)
{
    if (*flag == -2)
    {
        if (str[0] == '\'')
        {
            *flag = 1;

            return;
        }
    }

    if (*flag == -3)
    {
        if (str[0] == '\"')
        {
            *flag = 1;

            return;
        }
    }

    if (*flag == -4)
    {
        if (strncmp(str, "*/", strlen("*/")) == 0)
        {
            *flag = 2;

            return;
        }
    }

    return;
}

int convert_255(int value)
{
    value = (value * 1000) / 255;

    return value;
}

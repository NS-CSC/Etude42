#include <ncurses.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <wctype.h>

//#include "config.h"
#include "display.h"
//#include "file_manager.h"
#include "input_handler.h"

void render_screen(wchar_t **file_data, const int current_max_lines);
// ファイルの中身をポインタ配列で渡すとそれを画面に表示する関数
void update_screen(wchar_t **display_lines, int display_line_count, const int *display_orig, const int *display_offsets, int current_scroll, int window_y, int indent_offset, const int *line_in_block, wchar_t **file_data);
// 表示用の行データを画面に描画する関数
void init();
// ncursesの初期設定をまとめた関数
int get_digits(int number);
// 十進数の桁数を求める関数
int get_display_width(char *str);
// 画面上の表示幅を取得する関数
wchar_t **get_display_str(wchar_t **file_data, int str_max_index, int window_x, int *display_str_max_index, int **display_orig, int **display_offsets);
// 画面の横幅に応じて改行した後の文字列のポインタを返す関数
int wchar_string_display_width(wchar_t *str);
// 無効な文字を無視するwcswidth

#define BACK_GROUND 0

#define COMMENT 1
#define CHAR 2
#define MACRO 3
#define VARIABLE 4
#define CONTROL 5
#define FUNCTION 6
#define NUMBER 7
#define WHITE 8

static int g_use_color = 0;

static int convert_255(int value);
static int is_word_char(wchar_t ch);
static int is_preprocessor_line(const wchar_t *line);
static int match_word(const wchar_t *str, const wchar_t *word);
static void compute_block_comment_states(wchar_t **file_data, int line_count, int *line_in_block);
static void scan_state_to_offset(const wchar_t *line, int offset, int in_block_start, int *in_block, int *in_string, int *in_char, int *in_line_comment);
static void render_highlighted_segment(const wchar_t *segment, const wchar_t *full_line, int offset, int in_block_start);
static void set_color_pair(int pair);
static void add_wstr(const wchar_t *str);
static void add_nwstr(const wchar_t *str, int n);

static const wchar_t *const macro_words[] = {
    L"__DATE__",
    L"__FILE__",
    L"__LINE__",
    L"__STDC__",
    L"__STDC_HOSTED__",
    L"__STDC_VERSION__",
    L"__TIME__",
    L"sizeof",
    L"_IOFBF",
    L"_IOLBF",
    L"_IONBF",
    L"BUFSIZ",
    L"EOF",
    L"FILENAME_MAX",
    L"FOPEN_MAX",
    L"L_tmpnam",
    L"NULL",
    L"SEEK_CUR",
    L"SEEK_END",
    L"SEEK_SET",
    L"stderr",
    L"stdin",
    L"stdout",
    L"EXIT_FAILURE",
    L"EXIT_SUCCESS",
    L"MB_CUR_MAX",
    L"RAND_MAX",
    NULL
};

static const wchar_t *const variable_words[] = {
    L"auto",
    L"char",
    L"const",
    L"double",
    L"enum",
    L"extern",
    L"float",
    L"int",
    L"long",
    L"register",
    L"short",
    L"signed",
    L"static",
    L"struct",
    L"typedef",
    L"union",
    L"unsigned",
    L"void",
    L"volatile",
    L"fpos_t",
    L"size_t",
    L"FILE",
    L"div_t",
    L"ldiv_t",
    L"wchar_t",
    NULL
};

static const wchar_t *const control_words[] = {
    L"break",
    L"case",
    L"continue",
    L"default",
    L"do",
    L"else",
    L"for",
    L"goto",
    L"if",
    L"return",
    L"switch",
    L"while",
    NULL
};

static const wchar_t *const function_words[] = {
    L"main",
    L"clearerr",
    L"fclose",
    L"feof",
    L"ferror",
    L"fflush",
    L"fgetc",
    L"fgetpos",
    L"fgets",
    L"fopen",
    L"fprintf",
    L"fputc",
    L"fputs",
    L"fread",
    L"freopen",
    L"fscanf",
    L"fseek",
    L"fsetpos",
    L"ftell",
    L"fwrite",
    L"getc",
    L"getchar",
    L"gets",
    L"perror",
    L"printf",
    L"putc",
    L"putchar",
    L"puts",
    L"remove",
    L"rename",
    L"rewind",
    L"scanf",
    L"setbuf",
    L"setvbuf",
    L"sprintf",
    L"sscanf",
    L"tmpfile",
    L"tmpnam",
    L"ungetc",
    L"vfprintf",
    L"vprintf",
    L"vsprintf",
    L"abort",
    L"abs",
    L"atexit",
    L"atof",
    L"atoi",
    L"atol",
    L"bsearch",
    L"calloc",
    L"div",
    L"exit",
    L"free",
    L"getenv",
    L"labs",
    L"ldiv",
    L"malloc",
    L"mblen",
    L"mbstowcs",
    L"mbtowc",
    L"qsort",
    L"rand",
    L"realloc",
    L"srand",
    L"strtod",
    L"strtol",
    L"strtoul",
    L"system",
    L"wcstombs",
    L"wctomb",
    L"memchr",
    L"memcmp",
    L"memcpy",
    L"memmove",
    L"memset",
    L"strcat",
    L"strchr",
    L"strcmp",
    L"strcoll",
    L"strcpy",
    L"strcspn",
    L"strerror",
    L"strlen",
    L"strncat",
    L"strncmp",
    L"strncpy",
    L"strpbrk",
    L"strrchr",
    L"strspn",
    L"strstr",
    L"strtok",
    L"strxfrm",
    NULL
};

void render_screen(wchar_t **file_data, const int current_max_lines)
{
    // ファイルの中身をポインタ配列で渡すとそれを画面に表示する関数
    // ディスプレイサイズによる描画の範囲（下限）が書いていないため、対応させる必要がある。

    int number;
    int indent_offset;
    int content_start_col;

    int window_x, window_y;

    int display_str_max_index;
    int i;

    int *display_orig;
    int *display_offsets;
    int *line_in_block;
    wchar_t **display_lines;

    init();

    getmaxyx(stdscr, window_y, window_x);


    indent_offset = get_digits(current_max_lines) + 1;
    content_start_col = indent_offset + 1;

    if (window_x - content_start_col < 2)
    {
        puts("有効な表示領域が存在しません");

        exit(1);
    }

    display_lines = get_display_str(file_data, current_max_lines, window_x - content_start_col, &display_str_max_index, &display_orig, &display_offsets);

    if (display_lines == NULL)
    {
        endwin();
        return;
    }

    line_in_block = malloc(sizeof(int) * current_max_lines);

    if (line_in_block == NULL)
    {
        endwin();
        return;
    }

    compute_block_comment_states(file_data, current_max_lines, line_in_block);

    update_screen(display_lines, display_str_max_index, display_orig, display_offsets, 0, window_y, indent_offset, line_in_block, file_data);
    input_handler(content_start_col, display_lines, display_str_max_index, window_x, window_y, display_orig, display_offsets, line_in_block, file_data);

    i = 0;
    while (i < display_str_max_index)
    {
        free(display_lines[i]);
        i++;
    }

    free(display_lines);
    free(display_orig);
    free(display_offsets);
    free(line_in_block);

    endwin();

    return;
}

void update_screen(wchar_t **display_lines, int display_line_count, const int *display_orig, const int *display_offsets, int current_scroll, int window_y, int indent_offset, const int *line_in_block, wchar_t **file_data)
{
    // ファイルの中身をポインタ配列で渡すとそれを画面に表示する関数

    int number;
    int index;
    int orig_index;

    set_color_pair(WHITE);
    if (g_use_color != 0)
    {
        bkgd(COLOR_PAIR(WHITE));
    }

    erase();
    // 描画した画面を削除

    number = 0;

    while (number < window_y && number + current_scroll < display_line_count)
    {
        index = number + current_scroll;

        move(number, 0);

        set_color_pair(WHITE);

        if (display_offsets[index] == 0)
        {
            printw("%*d ", indent_offset, display_orig[index] + 1);
        }
        else
        {
            printw("%*s ", indent_offset, "");
        }

        if (g_use_color == 0)
        {
            printw("%ls", display_lines[index]);
        }
        else
        {
            orig_index = display_orig[index];
            render_highlighted_segment(display_lines[index], file_data[orig_index], display_offsets[index], line_in_block[orig_index]);
        }

        clrtoeol();

        number++;
    }

    while (number < window_y)
    {
        move(number, 0);
        set_color_pair(WHITE);
        clrtoeol();
        number++;
    }

    refresh();

    return;
}

void init()
{
    // ncursesの初期設定をまとめた関数

    initscr();
    noecho();
    keypad(stdscr, TRUE);
    idlok(stdscr, FALSE);
    scrollok(stdscr, FALSE);

    g_use_color = 0;

    if (has_colors() == TRUE)
    {
        start_color();
        g_use_color = 1;

        if (can_change_color() == TRUE)
        {
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
        }
        else
        {
            init_pair(1, COLOR_BLUE, COLOR_BLACK);
            init_pair(2, COLOR_GREEN, COLOR_BLACK);
            init_pair(3, COLOR_CYAN, COLOR_BLACK);
            init_pair(4, COLOR_CYAN, COLOR_BLACK);
            init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
            init_pair(6, COLOR_BLUE, COLOR_BLACK);
            init_pair(7, COLOR_YELLOW, COLOR_BLACK);
            init_pair(8, COLOR_WHITE, COLOR_BLACK);
        }
    }

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

int wchar_string_display_width(wchar_t *str)
{
    int i;
    int length;
    int width_sum;

    i = 0;
    width_sum = 0;

    while (str[i] != L'\0')
    {
        length = wcwidth(str[i]);

        if (length > 0)
        {
            width_sum += length;
        }

        i++;
    }

    return width_sum;
}

wchar_t **get_display_str(wchar_t **file_data, int str_max_index, int window_x, int *display_str_max_index, int **display_orig, int **display_offsets)
{
    int index;
    int count;
    int capacity;
    int start;
    int end;
    int width;
    int char_width;

    wchar_t **display_str;
    wchar_t *current_str;
    wchar_t *segment;

    capacity = str_max_index * 2;
    if (capacity < 32)
    {
        capacity = 32;
    }

    display_str = malloc(sizeof(wchar_t *) * capacity);
    *display_orig = malloc(sizeof(int) * capacity);
    *display_offsets = malloc(sizeof(int) * capacity);

    if (display_str == NULL || *display_orig == NULL || *display_offsets == NULL)
    {
        free(display_str);
        free(*display_orig);
        free(*display_offsets);
        return NULL;
    }

    index = 0;
    count = 0;

    while (index < str_max_index)
    {
        current_str = file_data[index];
        start = 0;

        while (current_str[start] != L'\0')
        {
            width = 0;
            end = start;

            while (current_str[end] != L'\0')
            {
                char_width = wcwidth(current_str[end]);

                if (char_width < 0)
                {
                    char_width = 0;
                }

                if (width + char_width > window_x && end > start)
                {
                    break;
                }

                width += char_width;
                end++;

                if (width >= window_x)
                {
                    break;
                }
            }

            if (count == capacity)
            {
                capacity *= 2;
                display_str = realloc(display_str, sizeof(wchar_t *) * capacity);
                *display_orig = realloc(*display_orig, sizeof(int) * capacity);
                *display_offsets = realloc(*display_offsets, sizeof(int) * capacity);

                if (display_str == NULL || *display_orig == NULL || *display_offsets == NULL)
                {
                    return NULL;
                }
            }

            segment = malloc(sizeof(wchar_t) * (end - start + 1));
            if (segment == NULL)
            {
                return NULL;
            }

            wcsncpy(segment, current_str + start, end - start);
            segment[end - start] = L'\0';

            display_str[count] = segment;
            (*display_orig)[count] = index;
            (*display_offsets)[count] = start;
            count++;

            start = end;
        }

        if (current_str[0] == L'\0')
        {
            if (count == capacity)
            {
                capacity *= 2;
                display_str = realloc(display_str, sizeof(wchar_t *) * capacity);
                *display_orig = realloc(*display_orig, sizeof(int) * capacity);
                *display_offsets = realloc(*display_offsets, sizeof(int) * capacity);

                if (display_str == NULL || *display_orig == NULL || *display_offsets == NULL)
                {
                    return NULL;
                }
            }

            segment = malloc(sizeof(wchar_t) * 1);
            if (segment == NULL)
            {
                return NULL;
            }

            segment[0] = L'\0';
            display_str[count] = segment;
            (*display_orig)[count] = index;
            (*display_offsets)[count] = 0;
            count++;
        }

        index++;
    }

    *display_str_max_index = count;

    return display_str;
}

static void set_color_pair(int pair)
{
    if (g_use_color != 0)
    {
        attrset(COLOR_PAIR(pair));
    }
}

static void add_wstr(const wchar_t *str)
{
    size_t len;
    char *mb;

    len = wcstombs(NULL, str, 0);

    if (len == (size_t)-1)
    {
        while (*str != L'\0')
        {
            addch('?');
            str++;
        }

        return;
    }

    mb = malloc(len + 1);
    if (mb == NULL)
    {
        return;
    }

    wcstombs(mb, str, len + 1);
    addnstr(mb, (int)len);
    free(mb);
}

static void add_nwstr(const wchar_t *str, int n)
{
    wchar_t *tmp;

    if (n <= 0)
    {
        return;
    }

    tmp = malloc(sizeof(wchar_t) * (n + 1));
    if (tmp == NULL)
    {
        return;
    }

    wcsncpy(tmp, str, n);
    tmp[n] = L'\0';
    add_wstr(tmp);
    free(tmp);
}

static int convert_255(int value)
{
    value = (value * 1000) / 255;

    return value;
}

static int is_word_char(wchar_t ch)
{
    if (ch == L'_')
    {
        return 1;
    }

    if (iswalnum(ch) != 0)
    {
        return 1;
    }

    return 0;
}

static int is_preprocessor_line(const wchar_t *line)
{
    int i;

    i = 0;

    while (line[i] == L' ' || line[i] == L'\t')
    {
        i++;
    }

    return line[i] == L'#';
}

static int match_word(const wchar_t *str, const wchar_t *word)
{
    size_t len;

    len = wcslen(word);

    if (wcsncmp(str, word, len) != 0)
    {
        return 0;
    }

    if (is_word_char(str[len]) != 0)
    {
        return 0;
    }

    return 1;
}

static void compute_block_comment_states(wchar_t **file_data, int line_count, int *line_in_block)
{
    int line_index;
    int in_block;

    line_index = 0;
    in_block = 0;

    while (line_index < line_count)
    {
        const wchar_t *line;
        int i;
        int in_string;
        int in_char;
        int escaped;

        line_in_block[line_index] = in_block;
        line = file_data[line_index];
        i = 0;
        in_string = 0;
        in_char = 0;
        escaped = 0;

        while (line[i] != L'\0')
        {
            wchar_t ch;
            wchar_t next;

            ch = line[i];
            next = line[i + 1];

            if (in_block != 0)
            {
                if (ch == L'*' && next == L'/')
                {
                    in_block = 0;
                    i += 2;
                    continue;
                }

                i++;
                continue;
            }

            if (in_string != 0)
            {
                if (escaped == 0 && ch == L'"')
                {
                    in_string = 0;
                }

                if (ch == L'\\' && escaped == 0)
                {
                    escaped = 1;
                }
                else
                {
                    escaped = 0;
                }

                i++;
                continue;
            }

            if (in_char != 0)
            {
                if (escaped == 0 && ch == L'\'')
                {
                    in_char = 0;
                }

                if (ch == L'\\' && escaped == 0)
                {
                    escaped = 1;
                }
                else
                {
                    escaped = 0;
                }

                i++;
                continue;
            }

            if (ch == L'/' && next == L'/')
            {
                break;
            }

            if (ch == L'/' && next == L'*')
            {
                in_block = 1;
                i += 2;
                continue;
            }

            if (ch == L'"')
            {
                in_string = 1;
                i++;
                continue;
            }

            if (ch == L'\'')
            {
                in_char = 1;
                i++;
                continue;
            }

            i++;
        }

        line_index++;
    }
}

static void scan_state_to_offset(const wchar_t *line, int offset, int in_block_start, int *in_block, int *in_string, int *in_char, int *in_line_comment)
{
    int i;
    int escaped;

    *in_block = in_block_start;
    *in_string = 0;
    *in_char = 0;
    *in_line_comment = 0;
    escaped = 0;
    i = 0;

    while (i < offset && line[i] != L'\0')
    {
        wchar_t ch;
        wchar_t next;

        ch = line[i];
        next = line[i + 1];

        if (*in_block != 0)
        {
            if (ch == L'*' && next == L'/')
            {
                *in_block = 0;
                i += 2;
                continue;
            }

            i++;
            continue;
        }

        if (*in_string != 0)
        {
            if (escaped == 0 && ch == L'"')
            {
                *in_string = 0;
            }

            if (ch == L'\\' && escaped == 0)
            {
                escaped = 1;
            }
            else
            {
                escaped = 0;
            }

            i++;
            continue;
        }

        if (*in_char != 0)
        {
            if (escaped == 0 && ch == L'\'')
            {
                *in_char = 0;
            }

            if (ch == L'\\' && escaped == 0)
            {
                escaped = 1;
            }
            else
            {
                escaped = 0;
            }

            i++;
            continue;
        }

        if (ch == L'/' && next == L'/')
        {
            *in_line_comment = 1;
            return;
        }

        if (ch == L'/' && next == L'*')
        {
            *in_block = 1;
            i += 2;
            continue;
        }

        if (ch == L'"')
        {
            *in_string = 1;
            i++;
            continue;
        }

        if (ch == L'\'')
        {
            *in_char = 1;
            i++;
            continue;
        }

        i++;
    }
}

static void render_highlighted_segment(const wchar_t *segment, const wchar_t *full_line, int offset, int in_block_start)
{
    int i;
    int in_block;
    int in_string;
    int in_char;
    int in_line_comment;
    int is_preprocessor;
    int escaped;

    scan_state_to_offset(full_line, offset, in_block_start, &in_block, &in_string, &in_char, &in_line_comment);
    is_preprocessor = 0;
    if (in_block == 0 && in_line_comment == 0 && is_preprocessor_line(full_line) != 0)
    {
        is_preprocessor = 1;
    }

    if (in_line_comment != 0)
    {
        set_color_pair(COMMENT);
        add_wstr(segment);
        return;
    }

    if (is_preprocessor != 0)
    {
        set_color_pair(MACRO);
        add_wstr(segment);
        return;
    }

    i = 0;
    escaped = 0;

    while (segment[i] != L'\0')
    {
        wchar_t ch;
        wchar_t next;

        ch = segment[i];
        next = segment[i + 1];

        if (in_block != 0)
        {
            set_color_pair(COMMENT);

            if (ch == L'*' && next == L'/')
            {
                add_nwstr(segment + i, 2);
                i += 2;
                in_block = 0;
                continue;
            }

            add_nwstr(segment + i, 1);
            i++;
            continue;
        }

        if (in_string != 0)
        {
            set_color_pair(CHAR);
            add_nwstr(segment + i, 1);

            if (escaped == 0 && ch == L'"')
            {
                in_string = 0;
            }

            if (ch == L'\\' && escaped == 0)
            {
                escaped = 1;
            }
            else
            {
                escaped = 0;
            }

            i++;
            continue;
        }

        if (in_char != 0)
        {
            set_color_pair(CHAR);
            add_nwstr(segment + i, 1);

            if (escaped == 0 && ch == L'\'')
            {
                in_char = 0;
            }

            if (ch == L'\\' && escaped == 0)
            {
                escaped = 1;
            }
            else
            {
                escaped = 0;
            }

            i++;
            continue;
        }

        if (ch == L'/' && next == L'/')
        {
            set_color_pair(COMMENT);
            add_wstr(segment + i);
            return;
        }

        if (ch == L'/' && next == L'*')
        {
            set_color_pair(COMMENT);
            add_nwstr(segment + i, 2);
            i += 2;
            in_block = 1;
            continue;
        }

        if (ch == L'"')
        {
            set_color_pair(CHAR);
            add_nwstr(segment + i, 1);
            in_string = 1;
            i++;
            continue;
        }

        if (ch == L'\'')
        {
            set_color_pair(CHAR);
            add_nwstr(segment + i, 1);
            in_char = 1;
            i++;
            continue;
        }

        if (ch >= L'0' && ch <= L'9')
        {
            int prev_is_word;
            wchar_t prev;
            int num_len;

            prev = (i == 0 && offset > 0) ? full_line[offset - 1] : (i > 0 ? segment[i - 1] : L'\0');
            prev_is_word = is_word_char(prev);

            if (prev_is_word == 0)
            {
                num_len = 1;
                while (segment[i + num_len] >= L'0' && segment[i + num_len] <= L'9')
                {
                    num_len++;
                }

                set_color_pair(NUMBER);
                add_nwstr(segment + i, num_len);
                i += num_len;
                continue;
            }
        }

        if (is_word_char(ch) != 0)
        {
            int word_len;
            int k;
            const wchar_t *const *table;
            int matched_color;
            wchar_t prev;

            prev = (i == 0 && offset > 0) ? full_line[offset - 1] : (i > 0 ? segment[i - 1] : L'\0');

            if (is_word_char(prev) != 0)
            {
                set_color_pair(WHITE);
                add_nwstr(segment + i, 1);
                i++;
                continue;
            }

            word_len = 0;
            while (segment[i + word_len] != L'\0' && is_word_char(segment[i + word_len]) != 0)
            {
                word_len++;
            }

            matched_color = 0;

            table = macro_words;
            k = 0;
            while (table[k] != NULL)
            {
                if (match_word(segment + i, table[k]) != 0)
                {
                    matched_color = MACRO;
                    break;
                }
                k++;
            }

            if (matched_color == 0)
            {
                table = variable_words;
                k = 0;
                while (table[k] != NULL)
                {
                    if (match_word(segment + i, table[k]) != 0)
                    {
                        matched_color = VARIABLE;
                        break;
                    }
                    k++;
                }
            }

            if (matched_color == 0)
            {
                table = control_words;
                k = 0;
                while (table[k] != NULL)
                {
                    if (match_word(segment + i, table[k]) != 0)
                    {
                        matched_color = CONTROL;
                        break;
                    }
                    k++;
                }
            }

            if (matched_color == 0)
            {
                table = function_words;
                k = 0;
                while (table[k] != NULL)
                {
                    if (match_word(segment + i, table[k]) != 0)
                    {
                        matched_color = FUNCTION;
                        break;
                    }
                    k++;
                }
            }

            if (matched_color != 0)
            {
                set_color_pair(matched_color);
                add_nwstr(segment + i, word_len);
                i += word_len;
                continue;
            }

            set_color_pair(WHITE);
            add_nwstr(segment + i, word_len);
            i += word_len;
            continue;
        }

        if (ch == L'<' || ch == L'>' || ch == L'+' || ch == L'-' || ch == L'=' || ch == L'*' || ch == L'/' || ch == L'&' || ch == L'%' || ch == L'!' || ch == L'^' || ch == L'~')
        {
            set_color_pair(MACRO);
            add_nwstr(segment + i, 1);
            i++;
            continue;
        }

        set_color_pair(WHITE);
        add_nwstr(segment + i, 1);
        i++;
    }
}

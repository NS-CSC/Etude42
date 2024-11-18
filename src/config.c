#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// グローバル設定変数
int line_number_indication;
int warning_sign;
int indent_width;
int space_filling;

int apply_user_config(const char *input);

int main(void)
{
    // ファイルを開く
    FILE *file = fopen("config.txt", "r");
    if (file == NULL)
    {
        printf("設定ファイルが見つかりません。\n");
        return 0;
    }

    // 仮の設定内容
    const char *inputs[] = {
        "set line_number_indication 1",
        "set warning_sign 1",
        "set indent_width 4",
        "set space_filling 1",
        NULL // 配列終端
    };

    // 設定を適用
    for (int i = 0; inputs[i] != NULL; i++)
    {
        apply_user_config(inputs[i]);
    }

    // 確認出力
    printf("line_number_indication: %d\n", line_number_indication);
    printf("warning_sign: %d\n", warning_sign);
    printf("indent_width: %d\n", indent_width);
    printf("space_filling: %d\n", space_filling);

    fclose(file);
    return 0;
}

int apply_user_config(const char *input)
{
    char buffer[256];
    char expression[256];
    char sentence[256];

    sscanf(input, "%s %s %s", buffer, expression, sentence);

    // コマンド確認
    if (strcmp(buffer, "set") != 0)
    {
        printf("構文エラー: '%s' は無効なコマンドです。\n", buffer);
        return 1;
    }

    // 各設定項目を適用
    if (strcmp(expression, "line_number_indication") == 0)
    {
        line_number_indication = atoi(sentence);
    }
    else if (strcmp(expression, "warning_sign") == 0)
    {
        warning_sign = atoi(sentence);
    }
    else if (strcmp(expression, "indent_width") == 0)
    {
        indent_width = atoi(sentence);
    }
    else if (strcmp(expression, "space_filling") == 0)
    {
        space_filling = atoi(sentence);
    }
    else
    {
        printf("構文エラー: '%s' は無効な設定項目です。\n", expression);
        return 1;
    }

    return 0;
}

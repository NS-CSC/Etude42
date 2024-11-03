#include <stdio.h>
#include <string.h>

// #include "file_manager.h"
// #include "display.h"
// #include "input_handler.h"
// #include "config.h"
//  これらは他の進捗に応じて変更する

void start_editor(void);

int main(int argc, char *argv[])
{
    int commandline_arg_index;
    char *path_name;
    int path_arg_count;
    int single_dash_option_length;

    path_arg_count = 0;

    if (argc == 1)
    {
        // コマンドライン引数が入力されていない時の条件式

        puts("コマンドライン引数が入力されていません。");

        return 0;
    }

    commandline_arg_index = 1;
    path_name = NULL;

    while (commandline_arg_index < argc)
    {
        single_dash_option_length = 1;

        if (argv[commandline_arg_index][0] == '-')
        {
            // コマンドラインオプションかどうかを区別する条件式
            
            if (argv[commandline_arg_index][1] == '-')
            {
                if (strcmp(&argv[commandline_arg_index][2], "help") == 0)
                {
                    puts("helpに頼るな");
                    // 処理
                }

                else
                {
                    printf("%sは存在しないオプションです\n",
                        argv[commandline_arg_index]);
                    // 処理
                }
            }

            else
            {
                do
                {
                    switch (
                        argv[commandline_arg_index][single_dash_option_length])
                    {
                        case 'h':
                            puts("helpに頼るな");
                            // 処理
                            break;
                        default:
                            printf("%cは存在しないオプションです\n",
                                argv[commandline_arg_index]
                                    [single_dash_option_length]);
                            break;
                    }

                    single_dash_option_length++;
                } while (
                    argv[commandline_arg_index][single_dash_option_length] !=
                    '\0');

            }
        }

        else
        {
            // 引数にパスがあったらという条件式

            path_arg_count++;

            if (path_arg_count > 1)
            {
                // パスの複数入力があった時の条件式

                puts("パスが複数入力されています。");

                return 0;
            }
            
            path_name = argv[commandline_arg_index];
            // path_nameにpathを代入
        }

        commandline_arg_index++;
    }

    if (path_name != NULL)
    {
        // example_function(path_name);
        //  file_managerへファイルの読み込みを指示
        //  file_managerから他のファイルの関数を呼び出してエディタが起動する

        printf("path_name: %s\n", path_name);
        // path_nameのテスト
    }

    else
    {
        puts("パスが入力されていません");
        // オプションによって発生する分岐は上で行われるので、ここでパスが入力されていないのはエラーとみなす
        // なお、ここをelse ifに変えて別の実装をするのでも良い

        return 0;
    }

    return 0;
}

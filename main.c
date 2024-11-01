#include <stdio.h>

// #include "file_manager.h"
// #include "display.h"
// #include "input_handler.h"
// #include "config.h"
//  これらは他の進捗に応じて変更する。

void start_editor(void);

int main(int argc, char *argv[])
{
    int commandline_arg_index;

    char *path_name;

    int path_arg_count;

    commandline_arg_index = 1;

    path_arg_count = 0;

    if (argc == 1)
    {
        // コマンドライン引数が入力されていない時の条件式

        puts("コマンドライン引数が入力されていません。");

        return 0;
    }

    while (commandline_arg_index < argc)
    {
        if (argv[commandline_arg_index][0] == '-')
        {
            // コマンドラインオプションかどうかの区別する条件式

            // if (argv[commandline_arg_index][1] == 'h') {}
            //  コマンドラインオプション判別の例

            printf("オプション: %s\n", argv[commandline_arg_index]);
            // 入力されたオプションの確認(テスト)
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

            path_name = argv[1];
            // path_nameにpathを代入

            // example_function(path_name);
            //  file_managerへファイルの読み込みを指示

            printf("path_nameのテスト: %s\n", path_name);
            // path_nameのテスト
        }

        commandline_arg_index++;
    }

    // start_editor(0);
    //  エディターの動作を開始する
    //  int型でエディタの状態を指定する（通常は0でhelpは1など...）

    return 0;
}

void start_editor(void)
{
    // ユーザーからの入力を受け取り、カーソル操作やテキスト編集などの>
    // 機能をハンドリングする。また、プログラムの状態を管理し、必要に応じて画面を更新する役割を担う。

    printf("エディター動作テスト\n");

    return;
}

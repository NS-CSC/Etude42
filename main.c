#include <stdio.h>

// #include "file_manager.h"
// #include "display.h"
// #include "input_handler.h"
// #include "config.h"
//  これらは他の進捗に応じて変更する。

void start_editor(void);

int main(int argc, char *argv[])
{
    printf("コマンドライン引数(テスト): %s\n", argv[1]);
    // コマンドライン引数のテスト

    /*if (argv[1][0] == '-')
    {
    if (argv[1][1] == 'h') {}
    }*/
    // コマンドラインオプションの雛形

    char *path_name = argv[1];
    // example_function(path_name);
    //  file_managerへファイルの読み込みを指示

    printf("path_nameのテスト: %s\n", path_name);
    // path_nameのテスト

    start_editor();
    // エディターの動作を開始する。

    return 0;
}

void start_editor(void)
{
    // ユーザーからの入力を受け取り、カーソル操作やテキスト編集などの>
    // 機能をハンドリングする。また、プログラムの状態を管理し、必要に応じて画面を更新する役割を担う。

    printf("エディター動作テスト\n");

    return;
}

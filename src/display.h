#ifndef DISPLAY_H
#define DISPLAY_H

void render_screen(char *file_data[], int current_max_lines);
// ファイルの中身をポインタ配列で渡すとそれを画面に表示する関数
int get_digits(int number);
// 十進数の桁数を求める関数
void input_handler(void);
// 別の関数を参照すると手間がかかるため、一時的にショートカットとして使用する関数

#endif  // DISPLAY_H

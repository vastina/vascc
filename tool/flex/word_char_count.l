/* word_char_counter.l */
/* 在这里我们要统计字符数和单词数，因此需要声明这两个统计变量 */
%{
#include <string.h>
int chars = 0;
int words = 0;
%}
/* 遇到匹配的模式则累加对应的统计变量 */
%%
[a-zA-Z]+ { chars += strlen(yytext); words++; }
. { }
%%

int main(int argc, char **argv) {
    yylex();
    printf("%d words of %d chars found\n", words, chars);
    return 0;
}

/*
usage:
flex word_char_count.l
gcc lex.yy.c -o word_char_count.out -lfl
*/
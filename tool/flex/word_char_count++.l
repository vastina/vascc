%option c++
%option noyywrap

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
    FlexLexer* lexer = new yyFlexLexer();
    lexer->yylex();
    std::cout << std::endl << words << " words of " << chars << " chars found." << std::endl;
    return 0;
}

/*
usage:
flex word_char_count++.l
g++ lex.yy.cc -o word_char_count++.out (-lfl is optional here)
*/
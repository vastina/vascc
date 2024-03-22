#ifndef _LEXER_H_
#define _LEXER_H_

#include <iostream>
#include <string>
#include <vector>

#include <cstdlib>
#include <cstring>


namespace vastina{

enum TOKEN{
    INT,
    MAIN,
    NUMBER,
    NLBRAC,     //左小括号
    NRBRAC,
    OBRACE,     //左花括号，open brace
    CBRACE,     //       close brace
    RETURN,
    SEMICOLON,  //分号
    VAR,
    UNKNOW
};

enum STATE{
    END = -1,
    NORMAL,
    ERROR
    //todo
};

struct token_t{
    TOKEN token;
    std::string data;

    token_t(TOKEN) ;
    token_t(TOKEN, std::string_view);
} ;



class lexer{

private:
    std::vector<token_t> tokens;
    std::string buffer;
    unsigned offset;
    STATE state;
    
    STATE ParseWhiteSpace();
public:
    lexer() = default;
    lexer(const char* filename);
    ~lexer();

    
    STATE Next();

    const std::vector<token_t> *getTokens();
};


}

#endif
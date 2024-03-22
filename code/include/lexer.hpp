#ifndef _LEXER_H_
#define _LEXER_H_

#include <iostream>
#include <string>
#include <vector>

#include <cstdlib>
#include <cstring>


namespace vastina{

enum TOKEN{
    UNKNOW = -1,
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

    token_t(TOKEN tk) ;
    token_t(TOKEN tk, const std::string& sv);
    token_t(TOKEN tk, std::string&& sv);
} ;



class lexer{

private:
    std::vector<token_t> tokens;
    std::string buffer;
    unsigned offset;
    STATE state;
    
    STATE ParseWhiteSpace();
    void whatname(const std::string&, TOKEN, bool(char endsymbol), TOKEN Default, bool(char endsymbol));
public:
    lexer() = default;
    lexer(const char* filename);
    ~lexer();

    
    STATE Next();

    const std::vector<token_t> *getTokens();
};


}

#endif
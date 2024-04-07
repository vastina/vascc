#ifndef _LEXER_H_
#define _LEXER_H_


#include <string>
#include <vector>
#include <functional>

#include "base/vasdef.hpp"


namespace vastina{


struct token_t{
    TOKEN token;
    std::string_view data;
    unsigned line;

//some were here just because I am lazy to delete them
    token_t(TOKEN tk) ;
    token_t(TOKEN tk, const std::string_view& sv);
    token_t(TOKEN tk, std::string_view&& sv);
    token_t(TOKEN tk, const std::string_view& sv, unsigned _line);
    token_t(TOKEN tk, std::string_view&& sv, unsigned _line);

    token_t(const token_t& tk);
    token_t(token_t&& tk);
} ;

class lexer{

public:
//only local use enum
enum STATE{
    END = -1,
    NORMAL,
    ERROR
    //todo
};
enum RESULT{
    SUCCESS,
    FAIL
};

private:
    std::vector<token_t> tokens;
    std::string buffer;
    unsigned offset;
    unsigned line;
    unsigned lineoffset;//to do
    STATE state;
    
    STATE ParseWhiteSpace();
    RESULT ParseKeyWord(const std::string_view&, TOKEN, std::function<bool(char)>, TOKEN Default, std::function<bool(char)>);
    //todo: parse number
    void ParseNumber();
    void forSingelWord(const std::string_view& target, TOKEN target_type);
    
public:

    lexer() = default;
    lexer(const char* filename);
    ~lexer();

    
    STATE Next();

    const std::vector<token_t>& getTokens();
    const std::string_view getBuffer();
};


}

#endif
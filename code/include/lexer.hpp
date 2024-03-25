#ifndef _LEXER_H_
#define _LEXER_H_


#include <string>
#include <vector>


#include "base/vasdef.hpp"


namespace vastina{


struct token_t{
    TOKEN token;
    std::string data;
    unsigned line;

    token_t(TOKEN tk) ;
    token_t(TOKEN tk, const std::string& sv);
    token_t(TOKEN tk, std::string&& sv);
    token_t(TOKEN tk, const std::string& sv, unsigned _line);
    token_t(TOKEN tk, std::string&& sv, unsigned _line);
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
    RESULT ParseKeyWord(const std::string&, TOKEN, bool(char endsymbol), TOKEN Default, bool(char endsymbol));
    //todo: add enum
    void ParseNumber();
    void forSingelWord(const std::string& target, TOKEN target_type);
    
public:

    lexer() = default;
    lexer(const char* filename);
    ~lexer();

    
    STATE Next();

    const std::vector<token_t>& getTokens();
    const std::string& getBuffer();
};


}

#endif
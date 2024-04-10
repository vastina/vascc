#ifndef _LEXER_H_
#define _LEXER_H_


#include <string>
#include <vector>
#include <functional>

#include "base/vasdef.hpp"
//see symbol.hpp struct token_t
#include "symbol.hpp"


namespace vastina{

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
    unsigned lineoffset;//todo
    STATE state;

    Scope::pointer current_scope;
    
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

    //this is not good
    Scope::pointer getScope();
};


}

#endif
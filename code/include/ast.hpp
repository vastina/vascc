#ifndef _AST_H_
#define _AST_H_

#include "lexer.hpp"

namespace vastina{
//finite state machine, aka FSM


class Parser{

private:
    std::vector<token_t>& tokens;


public:
    Parser(std::vector<token_t>& tks);
    ~Parser(){};

};


}//namespace vastina

#endif
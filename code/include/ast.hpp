#ifndef _AST_H_
#define _AST_H_

#include "lexer.hpp"

namespace vastina{
//finite state machine, aka FSM
//https://zh.cppreference.com/w/cpp/language/operator_precedence
typedef struct Expression{

} Expression;

typedef struct Statement{

} Statement;

typedef struct Block{
    Statement* statements;

} Block;


class Parser{

private:
    std::vector<token_t>& tokens;
    unsigned offset;

    std::vector<Block> blocks;

public:
    Parser(std::vector<token_t>& tks);
    ~Parser(){};

    token_t& Peek();
    void Eat();
    //these two should not be void, so todo
    void Except(const token_t& tk);
    void TryEat();

    void Stmt(Statement& stmt);

};


}//namespace vastina

#endif
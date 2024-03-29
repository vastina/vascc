#ifndef _AST_H_
#define _AST_H_

#include "base/vasdef.hpp"
#include "base/Tree.hpp"
#include "lexer.hpp"

#include <unordered_map>
#include <vector>

namespace vastina{
//finite state machine, aka FSM
//https://zh.cppreference.com/w/cpp/language/operator_precedence
typedef struct Expression{
    std::vector<token_t>& tokens;
    unsigned start;
    unsigned end;
} Expression;

typedef struct Statement{

} Statement;

typedef struct Block{
    std::vector<Statement> statements;
    unsigned offset;
} Block;



class MatchTable{

public:
    struct MatchUnit{
        TOKEN tk_type;
        FSM current_state;

        MatchUnit(TOKEN tk, FSM state):tk_type(tk), current_state(state){};
        bool operator==(const MatchUnit& m) const;
        bool operator!=(const MatchUnit& m) const;
    };

    struct Matcher{
        std::vector<MatchUnit> units;
        //maybe set is better?
        unsigned offset;
    };

private:
    MatchTable();
    std::unordered_map<TOKEN, Matcher> matchTable;

public:
    MatchTable(const MatchTable& m) = delete;
    MatchTable& operator=(const MatchTable& m) = delete;
    MatchTable(MatchTable&& m) = delete;
    MatchTable& operator=(MatchTable&& m) = delete;
    ~MatchTable();

    static MatchTable& getInstance(){
        static MatchTable instance;
        return instance;
    }

    void RegisteCase(TOKEN current_token, TOKEN except_token, FSM state);
    void UnregisteCase(TOKEN current_token, TOKEN except_token, FSM state);

    void Error(const char*);
    void Warn(const char*);

    const std::vector<MatchUnit>& getMatchList(TOKEN);
    TOKEN getNextMarch(TOKEN);
};

typedef struct before_main_t{
    before_main_t();
    ~before_main_t();
} before_main_t;

class Parser{

private:
    std::vector<token_t>& tokens;
    unsigned offset;

private:
    std::vector<Block> blocks;

    FSM current_state;

public:
    Parser(std::vector<token_t>& tks);
    ~Parser(){};

    token_t& Peek();
    void Eat();
    //these two should not be void? so todo
    void Except(const token_t& excepted);
    void TryEat(const token_t& excepted);

    void Stmt(Statement& stmt);

};

template<typename ty>
class baseExpression{
private:
    ty value;

public:
    baseExpression() = default;
    virtual ~baseExpression();
    virtual void Walk() = 0;
    virtual void Parse() = 0;
//  virtual std::string ToString() = 0;

public:
    const ty& getValue();
    void setValue(const ty&);
    void setValue(ty&&);
};

template<typename ty>
class CalExpression: public baseExpression<ty>{

};

template<typename ty>
class AssignExpression: public baseExpression<ty>{

};

template<typename ty>
class DeclExpression: public baseExpression<ty>{

};

template<typename ty>
class AddrExpression: public baseExpression<ty>{

};

}//namespace vastina

#endif
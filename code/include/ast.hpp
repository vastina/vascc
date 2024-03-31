#ifndef _AST_H_
#define _AST_H_

#include "base/vasdef.hpp"
#include "base/Tree.hpp"
#include "lexer.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

namespace vastina{
//finite state machine, aka FSM
//https://zh.cppreference.com/w/cpp/language/operator_precedence
using TokenPtr = //std::shared_ptr<std::vector<token_t>>;
    std::vector<token_t>*;//todo: use shared_ptr

typedef struct ExpressionUnit{
    TokenPtr tokens;
    unsigned start;
    unsigned end;

    ExpressionUnit() = delete;
    ExpressionUnit(TokenPtr tks, unsigned s, unsigned e);
    ~ExpressionUnit();
} ExpressionUnit;

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

//-----------------------------------------------------------------------------------------------------------------------
template<typename ty>
class baseExpression{
private:
    ty value_;

public:
    baseExpression() = default;
    virtual ~baseExpression() = default;
    virtual void Walk(walk_order) const =0;
    virtual void Parse() =0;
    virtual void Calculate() =0;

public:
    const ty getValue();
    void setValue(const ty&);
    void setValue(ty&&);
    
    //friend std::string_view ToString<ty>(const ty& t);
};

template<typename ty>
class CalExpression: public baseExpression<ty>{

public://they will not be created every time when a instance of CalExpression is created
    typedef struct _cal_node{
        token_t tk;
        unsigned level = 0;

        _cal_node(): tk(TOKEN::UNKNOW){};
        _cal_node(const token_t& _tk): tk(_tk){};
    } _cal_node;

    typedef TreeNode<_cal_node> cal_node;

    enum class cal_type{
        OPERATOR,
        VALUE,
        BRAC
    };

    constexpr cal_type cal_token_type(TOKEN tk){
        switch (tk)
        {
        case TOKEN::ADD:
        case TOKEN::NEG:
        case TOKEN::MULTI:
        case TOKEN::DIV:
        case TOKEN::LOGAND:
        case TOKEN::LOGNOT:
        case TOKEN::LOGOR :
            return cal_type::OPERATOR;
        case TOKEN::NUMBER:
            return cal_type::VALUE;
        case TOKEN::NLBRAC:
        case TOKEN::NRBRAC:
            return cal_type::BRAC;
        default:
            return cal_type::OPERATOR;
        }
    }

private:
    ExpressionUnit food_; //because it is to be eaten
    typename cal_node::pointer root_;
    ty value_;
    bool isConstexpr;
public:
    inline const ty getValue() const{
        return value_;
    }
    using baseExpression<ty>::setValue;
public:
    //CalExpression() = delete;
    CalExpression(ExpressionUnit&& e);

    void Walk(walk_order) const override;
    void Parse() override;
    void Calculate() override;

private:
    typename cal_node::pointer Parse_(unsigned &offset);
    ty Calculate_(const typename cal_node::pointer);

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

typedef struct BracketCount{
    unsigned open = 0;
    unsigned close = 0;
} BracketCount;

}//namespace vastina

#endif
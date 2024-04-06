#ifndef _AST_H_
#define _AST_H_

#include "base/vasdef.hpp"
#include "base/Tree.hpp"
#include "lexer.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

namespace vastina{
//finite state machine, aka FSM
//https://zh.cppreference.com/w/cpp/language/operator_precedence
using TokenPtr = //std::shared_ptr<std::vector<token_t>>;
    std::vector<token_t>*;//todo: use shared_ptr

typedef struct ExpressionUnit{
    TokenPtr tokens;
    unsigned start;
    unsigned end;

    ExpressionUnit(TokenPtr tks, unsigned s, unsigned e): tokens(tks), start(s), end(e){};
    ~ExpressionUnit(){
        if(nullptr != tokens) 
            tokens = nullptr;
    }
} ExpressionUnit;

typedef struct Statement{

} Statement;

typedef struct Block{
    std::vector<Statement> statements;
    unsigned offset;
} Block;



// class MatchTable{

// public:
//     struct MatchUnit{
//         TOKEN tk_type;
//         FSM current_state;

//         MatchUnit(TOKEN tk, FSM state):tk_type(tk), current_state(state){};
//         bool operator==(const MatchUnit& m) const;
//         bool operator!=(const MatchUnit& m) const;
//     };

//     struct Matcher{
//         std::vector<MatchUnit> units;
//         //maybe set is better?
//         unsigned offset;
//     };

// private:
//     MatchTable();
//     std::unordered_map<TOKEN, Matcher> matchTable;

// public:
//     MatchTable(const MatchTable& m) = delete;
//     MatchTable& operator=(const MatchTable& m) = delete;
//     MatchTable(MatchTable&& m) = delete;
//     MatchTable& operator=(MatchTable&& m) = delete;
//     ~MatchTable();

//     // static MatchTable& getInstance(){
//     //     static MatchTable instance;
//     //     return instance;
//     // }

//     void RegisteCase(TOKEN current_token, TOKEN except_token, FSM state);
//     void UnregisteCase(TOKEN current_token, TOKEN except_token, FSM state);

//     void Error(const char*);
//     void Warn(const char*);

//     const std::vector<MatchUnit>& getMatchList(TOKEN);
//     TOKEN getNextMarch(TOKEN);
// };

// typedef struct before_main_t{
//     before_main_t();
//     ~before_main_t();
// } before_main_t;

// class Parser{

// private:
//     std::vector<token_t>& tokens;
//     unsigned offset;

// private:
//     std::vector<Block> blocks;

//     FSM current_state;

// public:
//     Parser(std::vector<token_t>& tks);
//     ~Parser(){};

//     token_t& Peek();
//     void Eat();
//     //these two should not be void? so todo
//     void Except(const token_t& excepted);
//     void TryEat(const token_t& excepted);

//     void Stmt(Statement& stmt);

// };

//-----------------------------------------------------------------------------------------------------------------------
template<typename ty>
class baseExpression{
protected:
    ty value_;
    ExpressionUnit food_; //because it is to be eaten
public:
    baseExpression(const ExpressionUnit& e): value_(), food_(e){};
    baseExpression(ExpressionUnit&& e): value_(), food_(std::move(e)){};
    virtual ~baseExpression() = default;
    virtual void Walk(walk_order) const =0;
    virtual void Parse() =0;
    virtual void Calculate() =0;

public:
    inline const ty& getValue(){
        return value_;
    }
    inline void setValue(const ty& v){
        value_ = v;
    }
    void setValue(ty&& v){
        value_ = std::move(v);
    }
    
    //friend std::string_view ToString<ty>(const ty& t);
};

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
        BRAC,
        TYPE
    };

    constexpr cal_type cal_token_type(TOKEN tk){
        switch (tk)
        {
        case TOKEN::ADD:
        case TOKEN::NEG:
        case TOKEN::MULTI:
        case TOKEN::DIV:
        case TOKEN::LOGAND:
        case TOKEN::AND:
        case TOKEN::LOGNOT:
        case TOKEN::LOGOR :
        case TOKEN::OR:
            return cal_type::OPERATOR;
        case TOKEN::SYMBOL:
        case TOKEN::NUMBER:
            return cal_type::VALUE;
        case TOKEN::NLBRAC:
        case TOKEN::NRBRAC:
            return cal_type::BRAC;
        case TOKEN::INT:
        case TOKEN::FLOAT:
        case TOKEN::DOUBLE:
        case TOKEN::BOOL:
        case TOKEN::CHAR:
            return cal_type::TYPE;
        default:
            return cal_type::OPERATOR;
        }
    }

template<typename ty>
class CalExpression: public baseExpression<ty>{

public://they will not be created every time when a instance of CalExpression is created
    ;

private:
    typename cal_node::pointer root_;
    bool isConstexpr;
public:
    using baseExpression<ty>::value_;
    using baseExpression<ty>::food_;
    using baseExpression<ty>::baseExpression;
    using baseExpression<ty>::getValue;
    using baseExpression<ty>::setValue;
public:
    //CalExpression() = delete;
    CalExpression(const ExpressionUnit& e): baseExpression<ty>(e), root_(nullptr), isConstexpr(){};
    CalExpression(ExpressionUnit&& e): baseExpression<ty>(std::move(e)), root_(nullptr), isConstexpr(){};

    inline void Walk(walk_order wo) const override{
        root_->Walk(wo, [](const _cal_node& data_){
            std::cout  << data_.tk.data  << '\n';
        });
    }
    inline void Parse() override{
        unsigned offset = food_.start;
        root_ = Parse_(offset);
    }
    void Calculate() override{
        value_ = Calculate_(root_);
        return;
    }

private:
    inline typename cal_node::pointer Parse_(unsigned &offset){
        auto root = new cal_node(food_.tokens->at(offset));
        root->data.level = Level(root->data.tk.token);
        if(offset >= food_.end) return root;
        
        while(true){
            auto current = new cal_node(food_.tokens->at(offset));
            offset++;
            switch (cal_token_type(current->data.tk.token)){
                case cal_type::BRAC:{
                    if(current->data.tk.token == TOKEN::NRBRAC){
                        root->data.level = 0;
                        return root;
                    }else {
                        if(cal_type::BRAC == cal_token_type(root->data.tk.token)) root = Parse_(offset);
                        else current = Parse_(offset);
                    }
                    break;
                }
                case cal_type::VALUE:{
                    auto temp = root->FindChildR(
                        [](const typename cal_node::pointer _node) {return (_node->right == nullptr);}
                    );
                
                    temp->InsertRight(current);

                    break;
                }
                case cal_type::OPERATOR:{
                    current->data.level = Level(current->data.tk.token); 
                    if(current->data.level >= root->data.level){
                        root->ReplaceByL(current);
                        root = current;
                    }
                    else{
                        auto temp = root->FindChildR(
                            [&current](const typename cal_node::pointer _node)->bool {
                                return (_node->right == nullptr)||(_node->data.level <= current->data.level);
                        });
                        if(temp != root)//current取代原node，原node成为current的left
                            temp->ReplaceByL(current);
                        else
                            temp->InsertRight(current);
                    }

                    break;
                }

                default:
                    return nullptr;
            }
        
            if(offset >= food_.end) break;

        }

        return root;
    }
    inline const ty Calculate_(const typename cal_node::pointer root);


};
template<>
inline const int CalExpression<int>::Calculate_(const typename cal_node::pointer root){
    switch (root->data.tk.token) {
        case TOKEN::ADD:
            return Calculate_(root->left) + Calculate_(root->right);
            break;
        case TOKEN::NEG:
            return Calculate_(root->left) - Calculate_(root->right);
            break;
        case TOKEN::MULTI:
            return Calculate_(root->left) * Calculate_(root->right);
            break;
        case TOKEN::DIV:
            return Calculate_(root->left) / Calculate_(root->right);
            break;
        case TOKEN::NUMBER:
            return std::stoi(root->data.tk.data);;
            break;
        case TOKEN::AND:
            return Calculate_(root->left) & Calculate_(root->right);
            break;
        case TOKEN::OR:
            return Calculate_(root->left) | Calculate_(root->right);
            break;
        default:
            return 0;
            break;
    }
}

template<>
inline const float CalExpression<float>::Calculate_(const typename cal_node::pointer root) {
    switch (root->data.tk.token) {
        case TOKEN::ADD:
            return Calculate_(root->left) + Calculate_(root->right);
            break;
        case TOKEN::NEG:
            return Calculate_(root->left) - Calculate_(root->right);
            break;
        case TOKEN::MULTI:
            return Calculate_(root->left) * Calculate_(root->right);
            break;
        case TOKEN::DIV:
            return Calculate_(root->left) / Calculate_(root->right);
            break;
        case TOKEN::NUMBER:
            return std::stof(root->data.tk.data);;
            break;
        default:
            return 0;
            break;
    }
}


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
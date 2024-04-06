#ifndef _AST_H_
#define _AST_H_

#include "base/vasdef.hpp"
#include "base/Tree.hpp"
#include "lexer.hpp"
#include "symbol.hpp"

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



/*
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

*/

class Expression{
protected:
    ExpressionUnit food_; //because it is to be eaten
public:
    Expression(const ExpressionUnit& e): food_(e){};
    Expression(ExpressionUnit&& e): food_(std::move(e)){};
    virtual ~Expression() = default;
    virtual void Walk(walk_order) const =0;
    virtual void Parse() =0;
    virtual void Calculate() =0;
    
    //friend std::string_view ToString<ty>(const ty& t);
};



template<typename ty>
class CalExpression: public Expression{

public://they will not be created every time when a instance of CalExpression is created
    typedef struct _cal_node{
        token_t tk;//should use ref instead of copy
        unsigned level = 0;

        _cal_node(): tk(TOKEN::UNKNOW){};
        _cal_node(const token_t& _tk): tk(_tk){};
    } _cal_node;

    typedef TreeNode<_cal_node> cal_node;

private:
    ty value_;
    typename cal_node::pointer root_;
    bool isConstexpr;//todo
public:
    using Expression::food_;
    using Expression::Expression;
    inline const ty& getValue_ref(){ return value_;}
    inline const ty getValue_copy(){ return value_;}
    inline void setValue(const ty& v){ value_ = v;}
    inline void setValue(ty&& v){ value_ = std::move(v);}
public:
    CalExpression() = delete;
    CalExpression(const ExpressionUnit& e): Expression(e), root_(nullptr), isConstexpr(){};
    CalExpression(ExpressionUnit&& e): Expression(std::move(e)), root_(nullptr), isConstexpr(){};

    inline void Walk(walk_order wo) const override{
        root_->Walk(wo, [](const _cal_node& data_){
            std::cout  << data_.tk.data  << '\n';
        });
    }
    inline void Parse() override{
        unsigned offset = food_.start;
        root_ = Parse_(offset);
    }
    inline void Calculate() override{
        value_ = Calculate_(root_);
        return;
    }

private:

    //for example 3.14+42, 3.14+(float)42 or (int)3.14+42
    inline void fallback(){
        

    };

    inline typename cal_node::pointer Parse_(unsigned &offset){
        auto root = new cal_node(food_.tokens->at(offset));
        root->data.level = Level(root->data.tk.token);
        if(offset >= food_.end) return root;
        
        while(true){
            auto current = new cal_node(food_.tokens->at(offset));
            offset++;
            switch (token_type(current->data.tk.token)){
                case TOKEN_TYPE::BRAC:{
                    if(current->data.tk.token == TOKEN::NRBRAC){
                        root->data.level = 0;
                        return root;
                    }else {
                        if(TOKEN_TYPE::BRAC == token_type(root->data.tk.token)) root = Parse_(offset);
                        else current = Parse_(offset);
                    }
                    break;
                }
                case TOKEN_TYPE::VALUE:{
                    auto temp = root->FindChildR(
                        [](const typename cal_node::pointer _node) {return (_node->right == nullptr);}
                    );
                
                    temp->InsertRight(current);

                    break;
                }
                case TOKEN_TYPE::OPERATOR:{
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
class AssignExpression: public Expression{
public:
    typedef struct _assign_node{
        // ty val;
        // const token_t& tk;
        // _assign_node() =delete;
        // _assign_node(const token_t& _tk): val(), tk(_tk){};
        // _assign_node(const token_t& _tk, const ty& _val): val(_val), tk(_tk) {};
    } _assign_node;

    typedef TreeNode<_assign_node> assign_node;
private:
    ty value_;
    typename assign_node::pointer root_;
public:
    using Expression::food_;
    using Expression::Expression;
    inline const ty& getValue_ref(){ return value_;}
    inline const ty getValue_copy(){ return value_;}
    inline void setValue(const ty& v){ value_ = v;}
    inline void setValue(ty&& v){ value_ = std::move(v);}
public:
    AssignExpression()=delete;
    AssignExpression(const ExpressionUnit& e): Expression(e), root_(nullptr){};
    AssignExpression(ExpressionUnit&& e): Expression(std::move(e)), root_(nullptr){};

    inline void Walk(walk_order wo) const override{
        root_->Walk(wo, [](const _assign_node& data_){
            std::cout  << data_.tk.data  << '\n';
        });
    }
    inline void Parse() override{
        unsigned offset = food_.start;
        root_ = Parse_(offset);
    }
    inline void Calculate() override{
        value_ = Calculate_(root_);
        return;
    }
private:
    inline typename assign_node::pointer Parse_(unsigned &offset){

    }

};

template<typename ty>
class DeclExpression: public Expression{

};

template<typename ty>
class AddrExpression: public Expression{

};

typedef struct BracketCount{
    unsigned open = 0;
    unsigned close = 0;
} BracketCount;

}//namespace vastina

#endif
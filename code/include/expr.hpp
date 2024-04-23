#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#include "base/Tree.hpp"
#include "base/vasdef.hpp"
#include "symbol.hpp"

#include <cstdint>
#include <iostream>
#include <memory>
#include <string_view>
#include <type_traits>

namespace vastina {

/*
// class MatchTable{

// public:
//     struct MatchUnit{
//         TOKEN tk_type;
//         FSM current_state;

//         MatchUnit(TOKEN tk, FSM state):tk_type(tk),
current_state(state){};
//         bool operator==(const MatchUnit& m) const;
//         bool operator!=(const MatchUnit& m) const;
//     };

//     struct Matcher{
//         std::vector<MatchUnit> units;
//         //maybe set is better?
//         u32 offset;
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

//     void RegisteCase(TOKEN current_token, TOKEN except_token, FSM
state);
//     void UnregisteCase(TOKEN current_token, TOKEN except_token, FSM
state);

//     void Error(const char*);
//     void Warn(const char*);

//     const std::vector<MatchUnit>& getMatchList(TOKEN);
//     TOKEN getNextMarch(TOKEN);
// };

*/

using std::make_shared;
using std::shared_ptr;

// some classes here should be deleted

template <typename ty>
concept NotVoid = !std::is_same_v<ty, void>;

class Expression {
  protected:
    u32 level_;

  public:
    using pointer = Expression *;
    virtual void Walk() const {};
    virtual void Parse() {};
    // ValExpr and OpExpr
    virtual TOKEN getToken() { return {}; }
    virtual std::string_view getName() { return {}; };

    virtual ~Expression() = default;

  public:
    inline u32 getLevel() { return level_; };
    inline void setLevel(u32 level) { level_ = level; };
};

class OpExpr : public Expression {
  protected:
    const token_t &op_;

  public:
    using pointer = OpExpr *;
    OpExpr() = delete;
    OpExpr(const token_t &op) : op_(op){};
    static inline pointer Create(TOKEN tk) {
        return new OpExpr(tk);
    }

  public:
    inline void Walk() const override {};
    inline void Parse() override {};
    inline TOKEN getToken() override { return op_.token; };
    inline std::string_view getName() override { return op_.name; };
};

class ValExpr : public Expression {
  private:
    Value::pointer value_;
    const token_t val_{TOKEN::UNKNOW, {}, UINT32_MAX};

  public:
    ValExpr(Value::pointer val) : value_(val){};
    ValExpr(const token_t &tk) : val_(tk){};
    inline void Walk() const override {};
    inline void Parse() override {};
    inline TOKEN getToken() override { return val_.token; };
    inline std::string_view getName() override { return val_.name; };
};

// 不能被作为值赋值给别的lval 与 不求值表达式 不是一回事, 这里为了方便取的是前者
class nValExpr : public Expression {
};

class CallExpr : public ValExpr {
  public:
    inline void Parse() override {};
};

class vCallExpr : public nValExpr { // call void
};

class DeclExpr : public nValExpr {
};

class AddrExpr : public ValExpr {
};

typedef struct BracketCount {
    u32 open = 0;
    u32 close = 0;
} BracketCount;

} // namespace vastina

/*
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
public:
    CalExpression() = delete;
    CalExpression(const ExpressionUnit& e): Expression(e), root_(nullptr), isConstexpr(){};
    CalExpression(ExpressionUnit&& e): Expression(std::move(e)), root_(nullptr), isConstexpr(){};

    inline void Parse() override{
        unsigned offset = food_.start;
        root_ = Parse_(offset);
    }


private:

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
*/

#endif
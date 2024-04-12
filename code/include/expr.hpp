#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#include "base/Tree.hpp"
#include "symbol.hpp"

#include <iostream>
#include <concepts>
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

//     void RegisteCase(TOKEN current_token, TOKEN except_token, FSM
state);
//     void UnregisteCase(TOKEN current_token, TOKEN except_token, FSM
state);

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
template<typename ty>
std::ostream& PrintVal(const ty& val){
    return (std::cout << val) ;
};


class Expression {
  protected:
    //ExpressionUnit food_; // because it is to be eaten
  public:
    //Expression(const ExpressionUnit &e) : food_(e){};
    //Expression(ExpressionUnit &&e) : food_(std::move(e)){};
    virtual void Walk() const = 0;
    virtual void Parse() = 0;
    virtual ~Expression() = default;
};

class OpExpr : public Expression{

};

//不能被作为值赋值给别的lval 与 不求值表达式 不是一回事
template<typename ty>
class ValExpr : public Expression {

private:
    ty value_;

public:
    inline void Walk() const override{PrintVal(value_);}
    inline void Parse() override {};
    virtual ty Calculate() = 0;
};

class nValExpr : public Expression {

};

template<typename ty> 
concept NotVoid = !std::is_same_v<ty, void>;


template<typename ty> requires NotVoid<ty>
class CallExpr : public ValExpr<ty> {

public:
    inline void Walk(walk_order) const override{};
    inline void Parse() override {};
    inline ty Calculate(){return {};}
};

// template<>
// class CallExpr<void> : public nValExpr{};

class vCallExpr : public nValExpr{//call void

};

template<typename ty>
class AssignExpr : public Expression {
    
};

template<typename ty>
class DeclExpr : public nValExpr {

};

template<typename ty>
class AddrExpression : public ValExpr<ty> {

};



/*
using TokenPtr_ =            // std::shared_ptr<std::vector<token_t>>;
    std::vector<token_t> *; // todo: use shared_ptr

// see symbol.hpp Preprocess::p_token_t
typedef struct ExpressionUnit_ {
    TokenPtr_ tokens;
    unsigned start;
    unsigned end;

    ExpressionUnit_(TokenPtr_ tks, unsigned s, unsigned e)
        : tokens(tks), start(s), end(e){};
    ~ExpressionUnit_() {
        if (nullptr != tokens)
            tokens = nullptr;
    }
} ExpressionUnit_;

class Expression_ {
  protected:
    ExpressionUnit_ food_; // because it is to be eaten
  public:
    Expression_(const ExpressionUnit_ &e) : food_(e){};
    Expression_(ExpressionUnit_ &&e) : food_(std::move(e)){};
    virtual ~Expression_() = default;
    virtual void Walk(walk_order) const = 0;
    virtual void Parse() = 0;
    virtual void Calculate() = 0;
};

template <typename ty>
class CalExpression : public Expression_ {

  public:
    typedef struct _cal_node {
        const token_t &tk;
        unsigned level = 0;

        _cal_node(const token_t &_tk) : tk(_tk){};
    } _cal_node;

    typedef TreeNode<_cal_node> cal_node;

  protected:
    ty value_;
    typename cal_node::pointer root_;
    bool isConstexpr; // todo
    using Expression_::food_;

  public:
    using Expression_::Expression_;
    inline const ty &
    getValue_ref() {
        return value_;
    }
    inline const ty
    getValue_copy() {
        return value_;
    }
    inline void
    setValue(const ty &v) {
        value_ = v;
    }
    inline void
    setValue(ty &&v) {
        value_ = std::move(v);
    }

  public:
    CalExpression() = delete;
    CalExpression(const ExpressionUnit_ &e)
        : Expression_(e), root_(nullptr), isConstexpr(){};
    CalExpression(ExpressionUnit_ &&e)
        : Expression_(std::move(e)), root_(nullptr), isConstexpr(){};

    inline void
    Walk(walk_order wo) const override {
        root_->Walk(wo, [](const _cal_node &data_) {
            std::cout << data_.tk.data << '\n';
        });
    }
    inline void
    Parse() override {
        unsigned offset = food_.start;
        root_ = Parse_(offset);
    }
    // if contexpr, calculate it at compile time
    inline void
    Calculate() override {
        value_ = Calculate_(root_);
        return;
    }

  protected:
    // for example 3.14+42, 3.14+(double)42
    inline void fallback(){

    };

    inline typename cal_node::pointer
    Parse_(unsigned &offset) {
        auto root = new cal_node(food_.tokens->at(offset));
        root->data.level = Level(root->data.tk.token);
        if (offset >= food_.end)
            return root;

        while (true) {
            auto current = new cal_node(food_.tokens->at(offset));
            offset++;
            switch (token_type(current->data.tk.token)) {
            case TOKEN_TYPE::BRAC: {
                if (current->data.tk.token == TOKEN::NRBRAC) {
                    root->data.level = 0;
                    return root;
                } else {
                    if (TOKEN_TYPE::BRAC == token_type(root->data.tk.token))
                        root = Parse_(offset);
                    else
                        current = Parse_(offset);
                }
                break;
            }
            case TOKEN_TYPE::VALUE: {
                auto temp = root->FindChildR(
                    [](const typename cal_node::pointer _node) {
                        return (_node->right == nullptr);
                    });

                temp->InsertRight(current);

                break;
            }
            case TOKEN_TYPE::OPERATOR: {
                current->data.level = Level(current->data.tk.token);
                if (current->data.level >= root->data.level) {
                    root->ReplaceByL(current);
                    root = current;
                } else {
                    auto temp = root->FindChildR(
                        [&current](
                            const typename cal_node::pointer _node) -> bool {
                            return (_node->right == nullptr) || (_node->data.level <= current->data.level);
                        });
                    if (temp != root) // current取代原node，原node成为current的left
                        temp->ReplaceByL(current);
                    else
                        temp->InsertRight(current);
                }

                break;
            }

            default: {
                RETURN_NULL
            }
            }

            if (offset >= food_.end)
                break;
        }

        return root;
    }
    inline ty Calculate_(const typename cal_node::pointer root);
};

// todo
template <typename ty>
class ConstCalExpression : public CalExpression<ty> {
};

template <>
inline int
CalExpression<int>::Calculate_(const typename cal_node::pointer root) {
    switch (root->data.tk.token) {
    case TOKEN::EQUAL:
        return Calculate_(root->left) == Calculate_(root->right);
        break;
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
        return std::stoi(root->data.tk.data.data());
        break;
    case TOKEN::TRUE:
        return 1;
    case TOKEN::FALSE:
        return 0;
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


*/

typedef struct BracketCount {
    unsigned open = 0;
    unsigned close = 0;
} BracketCount;

} // namespace vastina

#endif
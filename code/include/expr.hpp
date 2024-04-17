#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#include "base/Tree.hpp"
#include "base/vasdef.hpp"
#include "symbol.hpp"

#include <iostream>
#include <memory>
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

template <typename ty>
std::ostream &PrintVal(const ty &val) {
    return (std::cout << val);
};

template <typename ty>
concept NotVoid = !std::is_same_v<ty, void>;

class Expression {
  protected:
    // ExpressionUnit food_; // because it is to be eaten
  public:
    using pointer = Expression *;
    // using pointer = shared_ptr<Expression>;
    // Expression(const ExpressionUnit &e) : food_(e){};
    // Expression(ExpressionUnit &&e) : food_(std::move(e)){};
    virtual void Walk() const {};
    virtual void Parse() {};
    // virtual pointer Create() { return pointer(); };
    virtual ~Expression() = default;
};

class OpExpr : public Expression {
  protected:
    TOKEN op_;

  public:
    using pointer = OpExpr *;
    OpExpr() : op_(TOKEN::UNKNOW){};
    OpExpr(TOKEN op) : op_(op){};
    static inline pointer Create() {
        return pointer();
        // return make_shared(OpExpr());
    }
    static inline pointer Create(TOKEN tk) {
        return pointer(tk);
    }

  public:
    inline void Walk() const override { std::cout << op_ << '\n'; };
    inline void Parse() override {};
};

template <typename ty>
    requires NotVoid<ty>
class ValExpr : public Expression {
  private:
    Value::pointer value_;

  public:
    inline void Walk() const override {
        //(void)PrintVal(ty());
    }
    inline void Parse() override {};
    virtual ty Calculate() = 0;
};

// 不能被作为值赋值给别的lval 与 不求值表达式 不是一回事, 这里为了方便取的是前者
class nValExpr : public Expression {
};

template <typename ty>
    requires NotVoid<ty>
class CallExpr : public ValExpr<ty> {
  public:
    inline void Walk(walk_order) const override{};
    inline void Parse() override {};
};

// template<>
// class CallExpr<void> : public nValExpr{};

class vCallExpr : public nValExpr { // call void
};

template <typename ty>
class DeclExpr : public nValExpr {
};

template <typename ty>
class AddrExpr : public ValExpr<ty> {
};

typedef struct BracketCount {
    u32 open = 0;
    u32 close = 0;
} BracketCount;

} // namespace vastina

#endif
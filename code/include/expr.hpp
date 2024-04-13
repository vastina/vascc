#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#include "base/Tree.hpp"
#include "symbol.hpp"

#include <concepts>
#include <iostream>
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

*/

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
    // Expression(const ExpressionUnit &e) : food_(e){};
    // Expression(ExpressionUnit &&e) : food_(std::move(e)){};
    virtual void Walk() const = 0;
    virtual void Parse() = 0;
    virtual ~Expression() = default;
};

class OpExpr : public Expression {
};

// 不能被作为值赋值给别的lval 与 不求值表达式 不是一回事
template <typename ty>
    requires NotVoid<ty>
class ValExpr : public Expression {

  private:
  public:
    inline void Walk() const override {}
    inline void Parse() override {};
    virtual ty Calculate() = 0;
};

class nValExpr : public Expression {
};

template <typename ty>
    requires NotVoid<ty>
class CallExpr : public ValExpr<ty> {

  public:
    inline void Walk(walk_order) const override{};
    inline void Parse() override {};
    inline ty Calculate() { return {}; }
};

// template<>
// class CallExpr<void> : public nValExpr{};

class vCallExpr : public nValExpr { // call void
};

template <typename ty>
class DeclExpr : public nValExpr {
};

template <typename ty>
class AddrExpression : public ValExpr<ty> {
};

typedef struct BracketCount {
    unsigned open = 0;
    unsigned close = 0;
} BracketCount;

} // namespace vastina

#endif
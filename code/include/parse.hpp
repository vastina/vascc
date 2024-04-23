#ifndef _PARSE_H_
#define _PARSE_H_

#include "base/vasdef.hpp"
#include "stmt.hpp"
#include "symbol.hpp"
#include "expr.hpp"
#include <vector>

namespace vastina {

//looks like Parser is just an interface
//no need of some members
class Parser {

  protected:
    const std::vector<token_t> &primary_tokens_;
    // u32 offset_{};
    const ptokens &processed_tokens_;
    u32 p_offset_{};

    // Stmts result_;
    // u32 id_{};

    Stmt::pointer current_stmt_;
    Scope::pointer scope_;

  public:
    Parser( const std::vector<token_t> &primary_tokens, 
            const ptokens &processed_tokens, 
            const Scope::pointer scope) : 
      primary_tokens_(primary_tokens),
      processed_tokens_(processed_tokens),
//      result_(),
      current_stmt_(new CompoundStmt(nullptr)),
      scope_(scope)
    {};

  private:
    inline P_TOKEN Current();
    inline const p_token_t &CurrentToken();
    inline P_TOKEN Peek();
    inline const p_token_t & PeekToken();
    inline void Next();
    u32 res_;
  
  public:
    void Walk();
    inline Stmt::pointer getStmtRoot(){return current_stmt_;} 

  public:
    i32 Parse();

    i32 Vdecl();
    i32 Fdecl();
    i32 Call();
    i32 Ret();
    i32 Ifer();
    i32 Loop();
    i32 Binary();

    //should this?
    static Expression::pointer Binary(range_t);
};

} // namespace vastina

#endif
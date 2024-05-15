#ifndef _PARSE_H_
#define _PARSE_H_

#include "base/vasdef.hpp"
#include "expr.hpp"
#include "stmt.hpp"
#include "symbol.hpp"
#include <vector>

namespace vastina {

class Parser
{

private:
  const std::vector<token_t>& primary_tokens_;
  // u32 offset_{}
  const ptokens& processed_tokens_;
  u32 p_offset_ {};

private:
  inline P_TOKEN Current();
  inline const p_token_t& CurrentToken();
  inline P_TOKEN Peek();
  inline const p_token_t& PeekToken();
  inline const p_token_t& Peekat( u32 );
  inline void Next();
  // peek PRimary_Tokens at
  inline const token_t& PeekPrtat( u32 );

private:
  Stmt::pointer current_stmt_;
  Scope::pointer scope_;

public:
  Parser( const std::vector<token_t>& primary_tokens, const ptokens& processed_tokens, const Scope::pointer scope )
    : primary_tokens_( primary_tokens )
    , processed_tokens_( processed_tokens )
    , current_stmt_( new CompoundStmt( nullptr ) )
    , scope_( scope )
  {}

public:
  void BfsWalk();
  void DfsWalk();
  inline Stmt::pointer getStmtRoot() { return current_stmt_; }

private:
  void doDfsWalk( Stmt::pointer, u32 = 1 );

public:
  i32 Parse();

private:
  i32 Vdecl();
  i32 Fdecl();
  i32 Call();
  i32 Ret();
  i32 Ifer();
  i32 Loop();
  i32 Binary();

  BinStmt::pointer Binary( range_t );
  CallExpr::pointer Callee( u32 );

  BinExpr::Node::pointer ParseBinary( u32&, const u32 );
};

} // namespace vastina

#endif
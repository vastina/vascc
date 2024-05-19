#ifndef _CODE_GENERATOR_H_
#define _CODE_GENERATOR_H_

#include "base/io.hpp"
#include "base/x86.hpp"
#include "stmt.hpp"
#include "symbol.hpp"
#include <stack>
#include <vector>

namespace vastina {

class Generator
{
public:
  typedef struct counter
  {
    u32 rsp {};
    typedef struct lc_
    {
      u32 lc {};     //.LC
      u32 pos { 1 }; // pos in buffer and 0 is x86::file_start
    } lc_;
    lc_ loc {};
    typedef struct lf_
    {
      u32 lfbe {};
      u32 pos {};
    } lf_;
    lf_ lf {};  //.LFB .LFE
    typedef struct jmp_{
      std::stack<u32> history {};
      u32 current {};
    } jmp_;
    jmp_ jmp {};
  } counter;

protected:
  Stmt::pointer current_stmt_;
  // the member isBreakable_ is still needed
  Scope::pointer scope_;

  counter counter_ {};

  Filer::pointer filer_ { nullptr };
  auto writer() { return static_cast<Writer*>( filer_ ); }

protected:
  string_view tlr_ { x86::r14 }; // temp left reg
  string_view trr_ { x86::r15 }; //      right

public:
  Generator() = delete;
  Generator( Stmt::pointer current_stmt, Scope::pointer scope ) : current_stmt_( current_stmt ), scope_( scope ) {}

protected:
  void doGenerate( Stmt::pointer );

protected:
  void IfStart( IfStmt::pointer );
  void IfEnd( );
  void LoopW( LoopStmt::pointer );
  void FuncStart( FdeclStmt::pointer );
  void FuncEnd( FdeclStmt::pointer );
  void Vdecl( VdeclStmt::pointer );
  void Binary( BinStmt::pointer, bool );
  void doBinary( BinExpr::Node::pointer );
  void Callee( CallStmt::pointer );
  void doCallee( CallExpr::pointer );
  void Params( const std::vector<Variable::pointer>& );
  void ParamClean( const std::vector<Variable::pointer>& );

protected:
  void poper( const string_view& );
  void pusher( const string_view& );

public:
  void Generate( const string_view& );
};

}; // namespace vastina

#endif
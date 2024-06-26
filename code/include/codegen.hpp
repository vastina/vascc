#ifndef _CODE_GENERATOR_H_
#define _CODE_GENERATOR_H_

#include "base/io.hpp"
#include "base/x86.hpp"
#include "stmt.hpp"
#include <stack>
#include <vector>

namespace vastina {

class Generator
{
public:
  typedef struct counter
  {
    typedef struct rsp_
    {
      // std::stack<u32> backup {};
      u32 current {};
    } rsp_;
    rsp_ rsp {};
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
    lf_ lf {}; //.LFB .LFE
    typedef struct jmp_
    {
      std::stack<u32> history {};
      u32 current {};
    } jmp_;
    jmp_ jmp {};
    typedef struct go_
    {
      std::stack<std::pair<u32, u32>> backup {};
      u32 goout {};  // for break
      u32 goback {}; // for continue
    } go_;
    go_ go {};
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
  ~Generator()
  {
    delete filer_;
    Scope::deleteScope( scope_ );
    Stmt::deleteStmt( current_stmt_ );
  }

protected:
  void doGenerate( Stmt::pointer );

protected:
  void RetGen( RetStmt::pointer );
  void IfStart( IfStmt::pointer );
  void IfEnd();
  void LoopWStart( LoopStmt::pointer );
  void LoopWEnd();
  void CondStart(); // condition start, todo
  void FuncStart( FdeclStmt::pointer );
  void FuncEnd( FdeclStmt::pointer );
  void doFuncEnd( FdeclStmt::pointer );
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
  void Clean();

public:
  void Generate( const string_view& );
};

}; // namespace vastina

#endif
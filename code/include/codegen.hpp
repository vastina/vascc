#ifndef _CODE_GENERATOR_H_
#define _CODE_GENERATOR_H_

#include "base/io.hpp"
#include "base/x86.hpp"
#include "stmt.hpp"
#include "symbol.hpp"
#include <vector>

namespace vastina {

class CodeGen
{
public:
  using pointer = CodeGen*;

protected:
  pointer parent_;
  Stmt::pointer stmt_;

public:
  CodeGen( pointer parent, Stmt::pointer stmt ) : parent_ { parent }, stmt_ { stmt } {}
  virtual ~CodeGen() = default;
  inline pointer getParent() const { return parent_; }
};

using CodeGens = std::vector<CodeGen::pointer>;
class BinaryGen : public CodeGen
{
public:
  using pointer = BinaryGen*;

protected:
public:
  BinaryGen( pointer parent, Stmt::pointer stmt ) : CodeGen( parent, stmt ) {}
};

class CompoundGen : public CodeGen
{
public:
  using pointer = CompoundGen*;

protected:
  CodeGens children_;

public:
  CompoundGen( pointer parent, Stmt::pointer stmt ) : CodeGen( parent, stmt ) {}
  virtual ~CompoundGen() { children_.clear(); }
};

class FdeclGen : public CompoundGen
{};

class VdeclGen : public CodeGen
{};

class CondGen : public CompoundGen
{};

class IfGen : public CondGen
{};

class LoopGen : public CondGen
{};

class RetGen : public CodeGen
{};

class CallGen : public CodeGen
{
public:
  using pointer = CallGen*;
};

class Generator
{
public:
  typedef struct counter
  {
    u32 rsp {};
    typedef struct lc_
    {
      u32 lc {};     //.LC
      u32 pos { 1 }; // pos in buffer and 0 is file start
    } lc_;
    lc_ loc {};
    typedef struct lf_
    {
      u32 lfbe {};
      u32 pos {};
    } lf_;
    lf_ lf {};  //.LFB .LFE
    u32 jmp {}; //.L
  } counter;

protected:
  Stmt::pointer current_stmt_;
  Scope::pointer scope_;

  counter counter_ {};

  CodeGen::pointer current_gener_;

  Filer::pointer filer_ { nullptr };
  auto writer() { return static_cast<Writer*>( filer_ ); }

protected:
  string_view tlr_ { x86::r14 }; // temp left reg
  string_view trr_ { x86::r15 }; //      right
  // string_view para_reg_ {r13}; // use when walk funciton call parameters

public:
  Generator() = delete;
  Generator( Stmt::pointer current_stmt, Scope::pointer scope )
    : current_stmt_( current_stmt ), scope_( scope ), current_gener_ { new CompoundGen( nullptr, current_stmt ) }
  {}

protected:
  i32 doGenerate( Stmt::pointer );

protected:
  void Binary( BinStmt::pointer );
  void doBinary( BinExpr::Node::pointer );
  void Callee( CallStmt::pointer );
  void doCallee( CallExpr::pointer );

public:
  void Generate( const string_view& );
};

}; // namespace vastina

#endif
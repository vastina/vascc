#ifndef _CODE_GENERATOR_H_
#define _CODE_GENERATOR_H_

#include "base/io.hpp"
#include "stmt.hpp"
#include <string_view>
#include <vector>

namespace vastina {

namespace x86 {

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
    u32 loc {};  //.LC
    u32 lfbe {}; //.LFB .LFE
  } counter;

protected:
  Stmt::pointer current_stmt_;
  Scope::pointer scope_;

  counter counter_ {};

  CodeGen::pointer current_gener_;

protected:
  string_view tlr_ { r15 }; // temp left reg
  string_view trr_ { r14 }; //      right
  // string_view para_reg_ {r13}; // use when walk funciton call parameters

public:
  Generator() = delete;
  Generator( Stmt::pointer current_stmt, Scope::pointer scope )
    : current_stmt_( current_stmt ), scope_( scope ), current_gener_ { new CompoundGen( nullptr, current_stmt ) }
  {}

protected:
  i32 Vdecl();
  i32 Fdecl();
  i32 Call();
  i32 Ret();
  i32 Ifer();
  i32 Loop();
  i32 Binary();

public:
  BinaryGen::pointer Binary( BinStmt::pointer );
  CallGen::pointer Callee( CallStmt::pointer );
};

}; // namespace x86

class CodeGener : public x86::Generator
{ // the interface
private:
  Writer::pointer writer_;

public:
  i32 Generate();
};

}; // namespace vastina

#endif
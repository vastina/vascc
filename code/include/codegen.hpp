#ifndef _CODE_GENERATOR_H_
#define _CODE_GENERATOR_H_

#include "base/String.hpp"
#include "base/io.hpp"
#include "stmt.hpp"

namespace vastina {

namespace x86 {

class CodeGen {
  public:
    using pointer = CodeGen *;
  
  protected:
    pointer parent_;
    Stmt::pointer stmt_;
  
  public:
    CodeGen(pointer parent, Stmt::pointer stmt) : parent_{parent}, stmt_{stmt} {}
    virtual ~CodeGen() = default;
    inline pointer getParent() const { return parent_; }
};


class BinaryGen : public CodeGen {
  public:
    using pointer = BinaryGen *;

};

class CompoundGen : public CodeGen {
  public:
    CompoundGen(pointer parent, Stmt::pointer stmt) : CodeGen(parent, stmt) {}

};

class FdeclGen : public CompoundGen {

};

class VdeclGen : public CodeGen {

};

class CondGen : public CompoundGen {

};

class IfGen : public CondGen {

};

class LoopGen : public CondGen {

};

class RetGen : public CodeGen {

};

class CallGen : public CodeGen {
  public:
    using pointer = CallGen *;

};

class Generator {

  protected:
    Stmt::pointer current_stmt_;
    Scope::pointer scope_;

    u32 rsp_use_counter_{}; //the number subl {}, %rsp
    u32 lc_use_counter_{};  //.LC{}

    CodeGen::pointer current_gener_;

  protected:
    string_view tlr_{r15}; // temp left reg
    string_view trr_{r14}; //      right

  public:
    Generator() = delete;
    Generator(Stmt::pointer current_stmt, Scope::pointer scope) : 
      current_stmt_(current_stmt),
      scope_(scope),
      current_gener_{new CompoundGen(nullptr, current_stmt)}
    {}

  protected:
    i32 Vdecl();
    i32 Fdecl();
    i32 Call();
    i32 Ret();
    i32 Ifer();
    i32 Loop();
    i32 Binary();

    BinaryGen::pointer Binary(BinStmt::pointer);
    CallGen::pointer Callee(CallStmt::pointer);

};

}; // namespace x86


class CodeGener : public x86::Generator { // the interface
  private:
    Writer::pointer writer_;
  public:
    i32 Generate();
};

}; // namespace vastina

#endif
#ifndef _X86_ASM_H_
#define _X86_ASM_H_

#include "base/vasdef.hpp"
#include <array>

namespace vastina {

class x86
{
public:
  x86( ... ) = delete;
  ~x86() = default;

public:
  const_str_t rsp { "%rsp" };
  const_str_t rbp { "%rbp" };
  const_str_t rip { "%rip" };
  const_str_t rax { "%rax" };
  const_str_t rbx { "%rbx" };
  const_str_t rcx { "%rcx" };
  const_str_t rdx { "%rdx" };
  const_str_t rdi { "%rdi" };
  const_str_t rsi { "%rsi" };
  const_str_t r8 { "%r8" };
  const_str_t r9 { "%r9" };
  const_str_t r10 { "%r10" };
  const_str_t r11 { "%r11" };
  const_str_t r12 { "%r12" };
  const_str_t r13 { "%r13" };
  const_str_t r14 { "%r14" };
  const_str_t r15 { "%r15" };

  const_str_t esp { "%esp" };
  const_str_t ebp { "%ebp" };
  const_str_t eip { "%eip" };
  const_str_t eax { "%eax" };
  const_str_t ebx { "%ebx" };
  const_str_t ecx { "%ecx" };
  const_str_t edx { "%edx" };
  const_str_t edi { "%edi" };
  const_str_t esi { "%esi" };
  const_str_t r8d { "%r8d" };
  const_str_t r9d { "%r9d" };
  const_str_t r10d { "%r10d" };
  const_str_t r11d { "%r11d" };
  const_str_t r12d { "%r12d" };
  const_str_t r13d { "%r13d" };
  const_str_t r14d { "%r14d" };
  const_str_t r15d { "%r15d" };

  // I don't think I would use this?
  // const_str_t sp{"%sp"};
  // const_str_t bp{"%bp"};
  // const_str_t ip{"%ip"};
  const_str_t ax { "%ax" };
  const_str_t bx { "bx" };
  const_str_t cx { "%cx" };
  const_str_t dx { "%dx" };
  const_str_t di { "%di" };
  const_str_t si { "%si" };
  const_str_t r8w { "%r8w" };
  const_str_t r9w { "%r9w" };
  const_str_t r10w { "%r10w" };
  const_str_t r11w { "%r11w" };
  const_str_t r12w { "%r12w" };
  const_str_t r13w { "%r13w" };
  const_str_t r14w { "%r14w" };
  const_str_t r15w { "%r15w" };

  // const_str_t spl{"%spl"};
  // const_str_t bpl{"%bpl"};
  // const_str_t ipl{"%ipl"};
  const_str_t al { "%al" };
  const_str_t ah { "%ah" };
  const_str_t bl { "%bl" };
  const_str_t bh { "%bh" };
  const_str_t cl { "%cl" };
  const_str_t ch { "%ch" };
  const_str_t dl { "%dl" };
  const_str_t dh { "%dh" };
  const_str_t dil { "%dil" };
  const_str_t sil { "%sil" };
  const_str_t r8b { "%r8b" };
  const_str_t r9b { "%r9b" };
  const_str_t r10b { "%r10b" };
  const_str_t r11b { "%r11b" };
  const_str_t r12b { "%r12b" };
  const_str_t r13b { "%r13b" };
  const_str_t r14b { "%r14b" };
  const_str_t r15b { "%r15b" };

  const_str_t sete { "sete" };
  const_str_t setne { "setne" };
  const_str_t setg { "setg" };
  const_str_t setge { "setge" };
  const_str_t setl { "setl" };
  const_str_t setle { "setle" };

  const_str_t mov { "mov" };
  const_str_t movw { "movw" };
  const_str_t movq { "movq" };
  const_str_t movl { "movl" };
  const_str_t movb { "movb" };
  const_str_t movss { "movss" };
  const_str_t movzbl { "movzbl" };

  const_str_t add { "add" };
  const_str_t addl { "addl" };
  const_str_t addq { "addq" };

  const_str_t sub { "sub" };
  const_str_t subl { "subl" };
  const_str_t subq { "subq" };

  const_str_t mul { "mul" };
  const_str_t imulq { "imulq" };

  const_str_t cqo { "cqo" };
  const_str_t cqto { "cqto" };
  const_str_t idiv { "idiv" };
  const_str_t idivq { "idivq" };

  const_str_t Not { "not" }; // not is used
  const_str_t notq { "notq" };

  const_str_t Xor { "xor" }; // xor is used
  const_str_t xorq { "xorq" };

  const_str_t orq { "orq" };

  const_str_t andl { "andl" };
  const_str_t andq { "andq" };

  const_str_t shlq { "shlq" };
  const_str_t shrq { "shrq" };

  const_str_t lea { "lea" };
  const_str_t leaq { "leaq" };

  const_str_t push { "push" };
  const_str_t pushq { "pushq" };
  const_str_t pop { "pop" };
  const_str_t popq { "popq" };
  const_str_t call { "call" };
  const_str_t ret { "ret" };

  const_str_t cmp { "cmp" };
  const_str_t cmpl { "cmpl" };
  const_str_t cmpq { "cmpq" };

  const_str_t je { "je" };
  const_str_t jne { "jne" };
  const_str_t jg { "jg" };
  const_str_t jge { "jge" };
  const_str_t jl { "jl" };
  const_str_t jle { "jle" };
  const_str_t jmp { "jmp" };

  const_str_t test { "test" };
  const_str_t testq { "testq" };

  constexpr static std::array<const string_view, 6> const regs_for_call { { rdi, rsi, rdx, rcx, r8, r9 } };
  // const_str_t regs_for_call[] { rdi, rsi, rdx, rcx, r8, r9 };

  // rax -> eax -> ax -> al
  static std::string to_lower( std::string reg );

  const_str_t file_start_ { "\t.file\t\"{}\"\n"
                            "\t.text\n" };
  static std::string file_start( const string_view& filename );

  // {} {} ->func_name
  const_str_t func_declare_start_ { "\t.globl\t{}\n"
                                    "\t.type\t{}, @function\n" };
  static std::string func_declare_start( const string_view& func_name );

  // {} -> counter, {} -> func_name
  const_str_t func_start_ { "{}:\n"
                            ".LFB{}:\n"
                            "\t.cfi_startproc\n"
                            "\tendbr64\n"
                            "\tpushq\t%rbp\n"
                            "\tmovq\t%rsp, %rbp\n" };
  static std::string func_start( const string_view& func_name, u32 counter );

  const_str_t func_end { "\tpopq\t%rbp\n"
                         "\tret\n" };

  // {} -> counter, {} {} -> func_name
  const_str_t func_declare_end_ { "\t.cfi_endproc\n"
                                  ".LFE{}:\n"
                                  "\t.size\t{}, .-{}\n" };
  static std::string func_declare_end( u32 counter, const string_view& func_name );

  const_str_t main_func_end { "\tleave\n"
                              "\tret\n" };

  // just keep it here
  const_str_t asm_end_str { "\t.section\t.note.GNU-stack,\"\",@progbits\n"
                            "\t.section\t.note.gnu.property,\"a\"\n"
                            "\t.align 8\n"
                            "\t.long\t1f - 0f\n"
                            "\t.long\t4f - 1f\n"
                            "\t.long\t5\n"
                            "0:\n"
                            "\t.string \"VAS\"\n"
                            "1:\n"
                            "\t.align 8\n"
                            "\t.long\t0xc0000002\n"
                            "\t.long\t3f - 2f\n"
                            "2:\n"
                            "\t.long\t0x3\n"
                            "3:\n"
                            "\t.align 8\n"
                            "4:\n\n" };

  const_str_t constant_ { "${}" };
  static std::string constant( i64 val );

  const_str_t loc_ { ".L{}:\n" };
  static std::string loc( u32 counter );

  // {} -> op, {},{} -> sth
  const_str_t Threer_ { "\t{}\t{}, {}\n" };
  static std::string Threer( const string_view& op, const string_view& _1, const string_view& _2 );

  // {} -> op, {} -> sth
  const_str_t Two_1 { "\t{}\t{}\n" };
  const_str_t Two_2 { "\t{} {}\n" };
  static std::string Twoer( const string_view& op, const string_view& _1 );
  static std::string Twoer_2( const string_view& op, const string_view& _1 );

  static std::string make_call( const string_view& func_name );
  static std::string call_builtin( const string_view& func_name );
  static std::string make_jump( const string_view& op, u32 loc );

  // {} -> value, {} -> register
  const_str_t regIndirect_ { "{}({})" };
  static std::string regIndirect( const string_view& val, const string_view& reg );

  // {} -> counter, {} -> type, {} -> data
  const_str_t rodata_ { "\t.section\t.rodata\n"
                        ".LC{}:\n"
                        "\t.{} {}\n"
                        "\t.text\n" };
  static std::string rodata( u32 counter, const string_view& type, const string_view& data );

  const_str_t single_ { "\t{}\n" };
  static std::string single( const string_view& op );

  // ${val}(${reg1},${reg2},${time})
  const_str_t lea_er_ { "{}({},{},{})" };
  static std::string leaer( const string_view& val,
                            const string_view& reg1,
                            const string_view& reg2,
                            const string_view& time );

  const_str_t to_neg_ { "\tnot\t{}\n"
                        "\tinc\t{}\n" };
  static std::string to_neg( const string_view& reg );

  const_str_t to_not_ {};

  const_str_t to_zero_ { "\txor\t{}, {}\n" };
  static std::string to_zero( const string_view& reg );
};

};

#endif
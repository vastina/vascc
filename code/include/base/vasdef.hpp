#ifndef _VASTINA_GLOBAL_DEFINE_
#define _VASTINA_GLOBAL_DEFINE_

#include <cstdint>
#include <string_view>

namespace vastina {

using u32 = std::uint32_t;
using i32 = std::int32_t;
using u64 = std::uint64_t;
using i64 = std::int64_t;

using std::string_view;

enum TOKEN
{
  UNKNOW = -1,

  UNSIGNED, // unsigned
  SIGNED,   // signed
  INT,      // int
  FLOAT,    // float
  DOUBLE,   // double
  BOOL,     // bool
  CHAR,     // char
  VOID,     // void
  LONG,     // long

  VAR,     // var
  LET,     // let
  SYMBOL,  // symbol
  FUNC,    // func
  SYMBOLF, // symbol
  MAIN,    // main
  RETURN,  // return

  STRUCT, // struct
  ENUM,   // enum

  CONST,  // const
  STATIC, // static
  EXTERN, // extern

  NUMBER, // number
  STRING, // string
  TRUE,   // true
  FALSE,  // false

  NLBRAC,    // (
  NRBRAC,    // )
  OBRACE,    // {
  CBRACE,    // }
  SEMICOLON, // ;
  COMMA,     // ,
  COLON,     // :
  DQUOTE,    // "
  SQUOTE,    // '

  GREATER, // >
  LOGNOT,  // !
  LESS,    // <
  AND,     // &
  LOGAND,  // &&
  OR,      // |
  LOGOR,   // ||
  OPS,     // ~
  XOR,     // ^

  NEG,    // -
  PLUS,   // +
  MULTI,  // *
  DIV,    // /
  LSHIFT, // <<
  RSHIFT, // >>
  MOD,    // %

  IF,       // if
  ELSE,     // else
  FOR,      // for
  WHILE,    // while
  DO,       // do
  SWITCH,   // switch
  CASE,     // case
  BREAK,    // break
  CONTINUE, // continue
  DEFAULT,  // default

  ASSIGN,       // =
  EQUAL,        // ==
  NOTEQUAL,     // !=
  GREATEREQUAL, // >=
  LESSEQUAL,    // <=
                // this is not good
  SUM,          // +=
  DIFF,         // -=
  MULTIE,       // *=
  DIVE,         // /=
  MODE,         // %=
  LSHIFTE,      // <<=
  RSHIFTE,      // >>=
  ANDE,         // &=
  XORE,         // ^=
  ORE,          // |=
  LINE,         // __LINE__
  FILE,         //

  // todo and more...
  //  ++    --    sizeof    &    *    ->    .    [    ]

  ASM // __asm__
};

// do not delete this if you really know why: the commit message is "preprocess_3"

enum class TOKEN_TYPE
{
  OPERATOR,
  VALUE,
  BRAC,
  TYPE,
  SEPERATOR // seperator, like comma, semicolon
};

inline static constexpr TOKEN_TYPE token_type( TOKEN tk )
{
  switch ( tk ) {
    case TOKEN::ANDE:
    case TOKEN::XORE:
    case TOKEN::ORE:
    case TOKEN::LSHIFTE:
    case TOKEN::RSHIFTE:
    case TOKEN::MULTIE:
    case TOKEN::DIVE:
    case TOKEN::MODE:
    case TOKEN::SUM:
    case TOKEN::DIFF:
    case TOKEN::ASSIGN:
    case TOKEN::LOGOR:
    case TOKEN::LOGAND:
    case TOKEN::OR:
    case TOKEN::XOR:
    case TOKEN::AND:
    case TOKEN::EQUAL:
    case TOKEN::NOTEQUAL:
    case TOKEN::GREATER:
    case TOKEN::GREATEREQUAL:
    case TOKEN::LESS:
    case TOKEN::LESSEQUAL:
    case TOKEN::LSHIFT:
    case TOKEN::RSHIFT:
    case TOKEN::PLUS:
    case TOKEN::NEG:
    case TOKEN::MULTI:
    case TOKEN::DIV:
    case TOKEN::MOD:
    case TOKEN::LOGNOT:
    case TOKEN::OPS:
      return TOKEN_TYPE::OPERATOR;
    case TOKEN::TRUE:
    case TOKEN::FALSE:
    case TOKEN::SYMBOL:
    case TOKEN::NUMBER:
    case TOKEN::STRING:
    case TOKEN::SYMBOLF: // what about void?
      return TOKEN_TYPE::VALUE;
    case TOKEN::NLBRAC:
    case TOKEN::NRBRAC:
    case TOKEN::OBRACE:
    case TOKEN::CBRACE:
      return TOKEN_TYPE::BRAC;
    case TOKEN::INT:
    case TOKEN::FLOAT:
    case TOKEN::DOUBLE:
    case TOKEN::BOOL:
    case TOKEN::CHAR:
      return TOKEN_TYPE::TYPE;
    case TOKEN::COMMA:
    case TOKEN::SEMICOLON:
      return TOKEN_TYPE::SEPERATOR;
    default:
      return TOKEN_TYPE::SEPERATOR;
  }
};

// https://zh.cppreference.com/w/c/language/operator_precedence
inline static constexpr u32 Level( TOKEN tk )
{
  switch ( tk ) {
    case TOKEN::ANDE:
    case TOKEN::XORE:
    case TOKEN::ORE:
      return 1 << 31; // &= ^= |=
    case TOKEN::LSHIFTE:
    case TOKEN::RSHIFTE:
      return 1 << 30; // <<= >>=
    case TOKEN::MULTIE:
    case TOKEN::DIVE:
    case TOKEN::MODE:
      return 1 << 29; // *= /= %=
    case TOKEN::SUM:
    case TOKEN::DIFF:
      return 1 << 28; // += -=
    case TOKEN::ASSIGN:
      return 1 << 27; // =
    // a?b:c
    // return 1<<26;
    case TOKEN::LOGOR:
      return 1 << 25; // ||
    case TOKEN::LOGAND:
      return 1 << 24; // &&
    case TOKEN::OR:
      return 1 << 23; // |
    case TOKEN::XOR:
      return 1 << 22; // ^
    case TOKEN::AND:
      return 1 << 21; // &
    case TOKEN::EQUAL:
    case TOKEN::NOTEQUAL:
      return 1 << 20; // == !=
    case TOKEN::GREATER:
    case TOKEN::GREATEREQUAL:
      return 1 << 19; // > >=
    case TOKEN::LESS:
    case TOKEN::LESSEQUAL:
      return 1 << 18; // < <=
    case TOKEN::LSHIFT:
    case TOKEN::RSHIFT:
      return 1 << 17; // << >>
    case TOKEN::PLUS:
    case TOKEN::NEG:
      return 1 << 16; // + -
    case TOKEN::MULTI:
    case TOKEN::DIV:
    case TOKEN::MOD:
      return 1 << 15; // * / %
    // sizeof
    // return 1<<14;
    // address
    // return 1<<13;
    //* for pointer
    // return 1<<12;
    // type cast
    // return 1<<11;
    case TOKEN::LOGNOT:
    case TOKEN::OPS:
      return 1 << 10; // ! ~
    // neg or pos, like -1919810, +114514
    // return 1<<9;
    // ++i, --i
    // return 1<<8;
    // struct->member
    // return 1<<7;
    // struct.member
    // return 1<<6;
    // array[index]
    // return 1<<5;
    case TOKEN::SYMBOLF: // function call
      return 1 << 4;
    // i++, i--
    // return 1<<3;
    case TOKEN::SYMBOL:
    case TOKEN::NUMBER:
    case TOKEN::STRING:
      return 1 << 2;
    default:
      return UINT32_MAX;
  }
}

enum class P_TOKEN
{ // processed token
  UNKNOW = -1,
  BINARY,
  FDECL,
  VDECL,
  ADDR,
  IF,
  LOOP,
  CALL,
  RET,
  END
};

// base sizeof
inline static constexpr u32 SIZEOF( TOKEN )
{
  return {};
}

namespace x86 {

#define const_str_t static constexpr string_view

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

const_str_t mul {"mul"};
const_str_t imulq {"imulq"};

const_str_t cqo {"cqo"};
const_str_t cqto {"cqto"};
const_str_t idiv {"idiv"};
const_str_t idivq {"idivq"};

const_str_t Xor { "xor" }; // xor is used

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

const_str_t je { "je" };
const_str_t jne { "jne" };
const_str_t jg { "jg" };
const_str_t jge { "jge" };
const_str_t jl { "jl" };
const_str_t jle { "jle" };
const_str_t jmp { "jmp" };
const_str_t test { "test" };

} // namespace x86

} // namespace vastina

#endif
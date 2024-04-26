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

enum TOKEN {
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

    STRUCT, // struct
    ENUM,   // enum

    CONST,  // const
    STATIC, // static

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

    MAIN,   // main
    RETURN, // return

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

enum class TOKEN_TYPE {
    OPERATOR,
    VALUE,
    BRAC,
    TYPE,
    SEPERATOR // seperator, like comma, semicolon
};

inline static constexpr TOKEN_TYPE token_type(TOKEN tk) {
    switch (tk) {
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
inline static constexpr u32 Level(TOKEN tk) {
    switch (tk) {
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

enum class P_TOKEN { // processed token
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
inline static constexpr u32 SIZEOF(TOKEN) {
    return {};
}

namespace x86 {

static constexpr string_view rax{"rax"};
static constexpr string_view rsp{"rsp"};
static constexpr string_view rbp{"rbp"};
static constexpr string_view rip{"rip"};
static constexpr string_view rdi{"rdi"};
static constexpr string_view rsi{"rsi"};
static constexpr string_view rdx{"rdx"};
static constexpr string_view rcx{"rcx"};
static constexpr string_view r08{"r8"};
static constexpr string_view r09{"r9"};
static constexpr string_view r10{"r10"};
static constexpr string_view r11{"r11"};
static constexpr string_view r12{"r12"};
static constexpr string_view r13{"r13"};
static constexpr string_view r14{"r14"};
static constexpr string_view r15{"r15"};

static constexpr string_view eax{"eax"};
static constexpr string_view esp{"esp"};
static constexpr string_view ebp{"ebp"};
static constexpr string_view eip{"eip"};
static constexpr string_view edi{"edi"};
static constexpr string_view esi{"esi"};
static constexpr string_view edx{"edx"};
static constexpr string_view ecx{"ecx"};
static constexpr string_view e08{"r8d"};
static constexpr string_view e09{"r9d"};
static constexpr string_view e10{"r10d"};
static constexpr string_view e11{"r11d"};
static constexpr string_view e12{"r12d"};
static constexpr string_view e13{"r13d"};
static constexpr string_view e14{"r14d"};
static constexpr string_view e15{"r15d"};

static constexpr string_view ax{"ax"};
static constexpr string_view sp{"sp"};
static constexpr string_view bp{"bp"};
static constexpr string_view ip{"ip"};
static constexpr string_view di{"di"};
static constexpr string_view si{"si"};
static constexpr string_view dx{"dx"};
static constexpr string_view cx{"cx"};
static constexpr string_view r08w{"r8w"};
static constexpr string_view r09w{"r9w"};
static constexpr string_view r10w{"r10w"};
static constexpr string_view r11w{"r11w"};
static constexpr string_view r12w{"r12w"};
static constexpr string_view r13w{"r13w"};
static constexpr string_view r14w{"r14w"};
static constexpr string_view r15w{"r15w"};

static constexpr string_view al{"al"};
static constexpr string_view ah{"ah"};
static constexpr string_view bl{"bl"};
static constexpr string_view bh{"bh"};
static constexpr string_view cl{"cl"};
static constexpr string_view ch{"ch"};
static constexpr string_view dl{"dl"};
static constexpr string_view dh{"dh"};

static constexpr string_view mov{"mov"};
static constexpr string_view movl{"movl"};
static constexpr string_view movq{"movq"};

static constexpr string_view add{"add"};
static constexpr string_view addl{"addl"};

static constexpr string_view sub{"sub"};
static constexpr string_view Xor{"xor"}; // xor is used
static constexpr string_view lea{"lea"};
static constexpr string_view push{"push"};
static constexpr string_view pop{"pop"};
static constexpr string_view call{"call"};
static constexpr string_view ret{"ret"};

static constexpr string_view cmp{"cmp"};
static constexpr string_view cmpl{"cmpl"};

static constexpr string_view je{"je"};
static constexpr string_view jne{"jne"};
static constexpr string_view jg{"jg"};
static constexpr string_view jge{"jge"};
static constexpr string_view jl{"jl"};
static constexpr string_view jle{"jle"};
static constexpr string_view jmp{"jmp"};
static constexpr string_view test{"test"};

} // namespace x86

} // namespace vastina

#endif
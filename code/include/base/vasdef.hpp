#ifndef _VASTINA_GLOBAL_DEFINE_
#define _VASTINA_GLOBAL_DEFINE_

#include <cstdint>

namespace vastina{

using u32 = std::uint32_t;
using i32 = std::int32_t;
using u64 = std::uint64_t;
using i64 = std::int64_t;


enum TOKEN{
    UNKNOW = -1,
//  unsigned  signed  int  float  double  bool  char  void
    UNSIGNED, SIGNED, INT, FLOAT, DOUBLE, BOOL, CHAR, VOID, LONG,
//  var  let          func  //类型后置确实有它的好处，let就先当作const var吧
    VAR, LET, SYMBOL, FUNC, SYMBOLF,
//  struct  enum
    STRUCT, ENUM, 
//  const  static
    CONST, //STATIC,
//  number  string  true  false
    NUMBER, STRING, TRUE, FALSE, 
//  (           )   {           }       ;         ,      :      "       '
    NLBRAC, NRBRAC, OBRACE, CBRACE, SEMICOLON, COMMA, COLON, DQUOTE, SQUOTE, 
//  main  return
    MAIN, RETURN,
//    >         !      <     &      &&    |    ||    ~    ^        
    GREATER, LOGNOT, LESS,  AND, LOGAND, OR, LOGOR, OPS, XOR,
//   -    +     *     /     >>      <<     %
    NEG, ADD, MULTI, DIV, LSHIFT, RSHIFT, MOD,
//  if  else  for  while  do  switch  case  break  continue  default
    IF, ELSE, FOR, WHILE, DO, SWITCH, CASE, BREAK, CONTINUE, DEFAULT,
//    =      ==      !=         >=             <=
    ASSIGN, EQUAL, NOTEQUAL, GREATEREQUAL, LESSEQUAL, 
//  +=    -=     *=     /=    %=     <<=     >>=      &=    ^=    |= this is not good
    SUM, DIFF, MULTIE, DIVE, MODE, LSHIFTE, RSHIFTE, ANDE, XORE, ORE,
// __LINE__ __FILE__
     LINE,    FILE,


//maybe in the future
    ASM
};


//do not delete this if you really know why: the commit message is "preprocess_3"

enum class TOKEN_TYPE{
    OPERATOR,
    VALUE,
    BRAC,
    TYPE,
    SEPERATOR //seperator, like comma, semicolon
};

inline static constexpr TOKEN_TYPE token_type(TOKEN tk){
    switch (tk)
    {
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
    case TOKEN::ADD:
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
    case TOKEN::SYMBOLF://what about void?
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

enum class EXPR{
    CAL,
    BINARY,
    DECL,
    ADDR,//address, something like a[0],&a
};

enum class STMT{
    IF,
    LOOP,
    CALL,
    RET,
};

//https://zh.cppreference.com/w/c/language/operator_precedence
inline static constexpr u32 Level(TOKEN tk){
    switch (tk){
        case TOKEN::ANDE: 
        case TOKEN::XORE: 
        case TOKEN::ORE:
            return 1<<31;// &= ^= |=
        case TOKEN::LSHIFTE:
        case TOKEN::RSHIFTE:
            return 1<<30;// <<= >>=
        case TOKEN::MULTIE:
        case TOKEN::DIVE:
        case TOKEN::MODE:
            return 1<<29;// *= /= %=
        case TOKEN::SUM:
        case TOKEN::DIFF:
            return 1<<28;// += -=
        case TOKEN::ASSIGN: 
            return 1<<27;// =
        // a?b:c 
            //return 1<<26;
        case TOKEN::LOGOR:
            return 1<<25;// ||
        case TOKEN::LOGAND:
            return 1<<24;// &&
        case TOKEN::OR:
            return 1<<23;// |
        case TOKEN::XOR:
            return 1<<22;// ^
        case TOKEN::AND:
            return 1<<21;// &
        case TOKEN::EQUAL:
        case TOKEN::NOTEQUAL:
            return 1<<20;// == !=
        case TOKEN::GREATER:
        case TOKEN::GREATEREQUAL:
            return 1<<19;// > >=
        case TOKEN::LESS:
        case TOKEN::LESSEQUAL:
            return 1<<18;// < <=
        case TOKEN::LSHIFT:
        case TOKEN::RSHIFT:
            return 1<<17;// << >>
        case TOKEN::ADD:
        case TOKEN::NEG:
            return 1<<16;// + -
        case TOKEN::MULTI:
        case TOKEN::DIV:
        case TOKEN::MOD:
            return 1<<15;// * / %
        //sizeof 
            //return 1<<14;
        // address 
            //return 1<<13;
        //* for pointer 
            //return 1<<12;
        //type cast 
            //return 1<<11;
        case TOKEN::LOGNOT:
        case TOKEN::OPS:
            return 1<<10;// ! ~
        //neg or pos, like -1919810, +114514 
            //return 1<<9;
        // ++i, --i 
            //return 1<<8;
        //struct->member 
            //return 1<<7;
        //struct.member  
            //return 1<<6;
        //array[index]   
            //return 1<<5;
        case TOKEN::SYMBOLF://function call
            return 1<<4;
        // i++, i-- 
            //return 1<<3;
        case TOKEN::SYMBOL:
        case TOKEN::NUMBER:
        case TOKEN::STRING:
            return 1<<2;
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

//base sizeof
inline static constexpr u32 SIZEOF(TOKEN ){
    return {};
}

}
#endif
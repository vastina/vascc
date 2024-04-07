#ifndef _VASTINA_GLOBAL_DEFINE_
#define _VASTINA_GLOBAL_DEFINE_

#include <string_view>
#include <typeinfo>

namespace vastina{

enum TOKEN{
    UNKNOW = -1,
    UNSIGNED, SIGNED, INT, FLOAT, DOUBLE, BOOL, CHAR, VOID,
//  unsigned  signed  int  float  double  bool  char  void
    VAR, LET, SYMBOL, FUNC, SYMBOLF,
//  var  let          func
//类型后置确实有它的好处，let就先当作const var吧
    STRUCT, ENUM, 
//  struct  enum
    CONST, //STATIC,
//  const  static
    NUMBER, STRING,
//  number  string
    NLBRAC, NRBRAC, OBRACE, CBRACE, SEMICOLON, COMMA, COLON,
//  (           )   {           }       ;         ,      :
    MAIN, RETURN,
//  main  return
    GREATER, LESS, NOT, LOGNOT, AND, LOGAND, OR, LOGOR, OPS,
// what is NOT since there is LOGNOT?                    ~
    NEG, ADD, MULTI, DIV, 
//   -    +     *     /
    IF, ELSE, FOR, WHILE, DO, SWITCH, CASE, BREAK, CONTINUE, DEFAULT,
//  if  else  for  while  do  switch  case  break  continue  default
    ASSIGN, EQUAL, NOTEQUAL, GREATEREQUAL, LESSEQUAL,//should these two?
//     =     ==      !=         >=             <=
    LINE, FILE,
// __LINE__ __FILE__


    ASM//maybe in the future
};



enum class TOKEN_TYPE{
    OPERATOR,
    VALUE,
    BRAC,
    TYPE,
    SEPERATOR //seperator, like comma, semicolon
};

constexpr TOKEN_TYPE token_type(TOKEN tk){
    switch (tk)
    {
    case TOKEN::ADD:
    case TOKEN::NEG:
    case TOKEN::MULTI:
    case TOKEN::DIV:
    case TOKEN::LOGAND:
    case TOKEN::AND:
    case TOKEN::LOGNOT:
    case TOKEN::LOGOR :
    case TOKEN::OR:
        return TOKEN_TYPE::OPERATOR;
    case TOKEN::SYMBOL:
    case TOKEN::NUMBER:
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
    ASSIGN,
    //CONDITION,//it is a type of calculate expression
    DECL,
    ADDR,//address, something like a[0],&a
};

enum class STMT{
    IF,
    GOTO,
    CALL,//可以为Calculate中的函数调用构造临时匿名变量？
    RET,
};

inline constexpr unsigned Level(TOKEN tk){
    switch (tk){
        case TOKEN::EQUAL:
            return 5;
        case TOKEN::ASSIGN:
            return 4;
        case TOKEN::ADD:
        case TOKEN::NEG:
            return 3;
        case TOKEN::AND:
        case TOKEN::NOT:
        case TOKEN::OR :
            return 2;
        case TOKEN::MULTI:
        case TOKEN::DIV:
            return 1;
        case TOKEN::SYMBOL:
        case TOKEN::NUMBER:
            return 0;
        default:
            return 1<<31;
    }
}

constexpr std::string_view _TypeName(std::string_view sv){
    if(sv.size()==0) return "";

    switch (sv[0]) {
        case 'i': return "int";
        case 'j': return "unsigned";
        case 'd': return "double";
        case 'c': return "char";
        case 'b': return "bool";
        case 'l': return "long";
        case 'v': return "void";
        
        default:  return "";
    }
}

template<typename ty>
constexpr std::string_view TypeName(const ty&){
    return _TypeName(typeid(ty).name());
}

#define RETURN_ERROR std::cerr << __FILE__ <<' '<<__LINE__ <<'\n'; \
                        return -1;

#define EXIT_ERROR std::cerr << __FILE__ <<' '<<__LINE__ <<'\n'; \
                        exit(-1);
}
#endif
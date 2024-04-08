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



//I actually don't need this
//commit and push to github then delete this function
//if I need it, I can get it back from github
constexpr std::string_view token_name(TOKEN tk){
    switch (tk)
    {
    case TOKEN::UNKNOW:
        return "UNKNOW";
    case TOKEN::UNSIGNED:
        return "unsigned";
    case TOKEN::SIGNED:
        return "signed";
    case TOKEN::INT:
        return "int";
    case TOKEN::FLOAT:
        return "float";
    case TOKEN::DOUBLE:
        return "double";
    case TOKEN::BOOL:
        return "bool";
    case TOKEN::CHAR:
        return "char";
    case TOKEN::VOID:
        return "void";
    case TOKEN::VAR: 
        return "var";
    case TOKEN::LET:
        return "let";
    case TOKEN::SYMBOL:
        return "symbol";
    case TOKEN::FUNC:
        return "func";
    case TOKEN::SYMBOLF:
        return "symbolf";
    case TOKEN::STRUCT:
        return "struct";
    case TOKEN::ENUM:
        return "enum";
    case TOKEN::CONST:
        return "const";
    case TOKEN::NUMBER:
        return "number";
    case TOKEN::STRING:
        return "string";
    case TOKEN::NLBRAC:
        return "(";
    case TOKEN::NRBRAC:
        return ")";
    case TOKEN::OBRACE:
        return "{";
    case TOKEN::CBRACE:
        return "}";
    case TOKEN::SEMICOLON:
        return ";";
    case TOKEN::COMMA:
        return ",";
    case TOKEN::COLON:
        return ":";
    case TOKEN::MAIN:
        return "main";
    case TOKEN::RETURN:
        return "return";
    case TOKEN::GREATER:
        return ">";
    case TOKEN::LESS:
        return "<";
    case TOKEN::NOT:
        return "!";
    // case TOKEN::LOGNOT:
    //     return "!!";
    case TOKEN::AND:
        return "&";
    case TOKEN::LOGAND:
        return "&&";
    case TOKEN::OR:
        return "|";
    case TOKEN::LOGOR:
        return "||";
    case TOKEN::OPS:
        return "~";
    case TOKEN::NEG:
        return "-";
    case TOKEN::ADD:
        return "+";
    case TOKEN::MULTI:
        return "*";
    case TOKEN::DIV:
        return "/";
    case TOKEN::IF:
        return "if";
    case TOKEN::ELSE:
        return "else";
    case TOKEN::FOR:
        return "for";
    case TOKEN::WHILE:
        return "while";
    case TOKEN::ASM:
        return "__asm__";
    default:
        return "UNKNOW";
    }
}
//do not delete this if you really know why: the commit message is "preprocess_3"

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
    case TOKEN::EQUAL:
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
    LOOP,
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
#define THIS_NOT_SUPPORT(sth) \
        std::cerr << sth << " not supported now\n"; \
        std::cerr << __FILE__ <<' '<<__LINE__ <<'\n'; \
        exit(-1);

}
#endif
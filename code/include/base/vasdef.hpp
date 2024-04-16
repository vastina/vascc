#ifndef _VASTINA_GLOBAL_DEFINE_
#define _VASTINA_GLOBAL_DEFINE_


namespace vastina{

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
//    >         !      <     &      &&     |    ||    ~           
    GREATER, LOGNOT, LESS,  AND, LOGAND, OR, LOGOR, OPS,
//   -    +     *     /
    NEG, ADD, MULTI, DIV, 
//  if  else  for  while  do  switch  case  break  continue  default
    IF, ELSE, FOR, WHILE, DO, SWITCH, CASE, BREAK, CONTINUE, DEFAULT,
//     =     ==      !=         >=             <=
    ASSIGN, EQUAL, NOTEQUAL, GREATEREQUAL, LESSEQUAL,
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
    case TOKEN::ADD:
    case TOKEN::NEG:
    case TOKEN::MULTI:
    case TOKEN::DIV:
    case TOKEN::EQUAL:
    case TOKEN::NOTEQUAL:
    case TOKEN::GREATEREQUAL:
    case TOKEN::LESSEQUAL:
    case TOKEN::GREATER:
    case TOKEN::LESS:
    case TOKEN::LOGNOT:
    case TOKEN::AND:
    case TOKEN::LOGAND:
    case TOKEN::OR:
    case TOKEN::LOGOR:
    case TOKEN::OPS:
    case TOKEN::ASSIGN:
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

//todo but not cpp https://zh.cppreference.com/w/cpp/language/operator_precedence
inline static constexpr unsigned Level(TOKEN tk){
    switch (tk){
        case TOKEN::EQUAL:
            return 5;
        case TOKEN::ASSIGN:
            return 4;
        case TOKEN::ADD:
        case TOKEN::NEG:
            return 3;
        case TOKEN::AND:
        case TOKEN::OPS:
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

//base sizeof
inline static constexpr unsigned SIZEOF(TOKEN ){
    return {};
}

}
#endif
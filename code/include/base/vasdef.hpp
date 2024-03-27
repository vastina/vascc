#ifndef _VASTINA_GLOBAL_DEFINE_
#define _VASTINA_GLOBAL_DEFINE_

namespace vastina{

enum TOKEN{
    UNKNOW = -1,
    UNSIGNED, SIGNED, INT, FLOAT, DOUBLE, BOOL, CHAR, VOID,
//  unsigned  signed  int  float  double  bool  char  void
    VAR, LET, FUNC, SYMBOL,
//  var  let  func
//类型后置确实有它的好处，let就先当作const var吧
    STRUCT, ENUM, 
//  struct  enum
    CONST, //STATIC,
//  const  static
    NUMBER, STRING,
//  number  string
    NLBRAC, NRBRAC, OBRACE, CBRACE, SEMICOLON,//分号
//  (           )   {           }       ;
    MAIN, RETURN,
//  main  return
    GREATER, LESS, NOT, LOGNOT, AND, LOGAND, OR, LOGOR,
// what is NOT since there is LOGNOT?
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


enum FSM{//finite state machine
    END = -1, START,
    ENTRY,//entry, maybe main, you can tell the compiler to start from other place
    DECL, //declaration
    ASSI,//assign operation
    CAL,// calculate something
    GOTO,// condition jump, including if else, for, while, do while, switch case  

    EXPR, EXPR1, EXPR2, EXPR3, EXPR4, EXPR5, EXPR6, EXPR7, EXPR8, EXPR9, EXPR10, 
    EXPR11, EXPR12, EXPR13, EXPR14, EXPR15, EXPR16, EXPR17, EXPR18, EXPR19, EXPR20, 
    EXPR21, EXPR22, EXPR23, EXPR24, EXPR25, EXPR26, EXPR27, EXPR28, EXPR29, EXPR30, 
    EXPR31, EXPR32, EXPR33, EXPR34, EXPR35, EXPR36, EXPR37, EXPR38, EXPR39, EXPR40, 
    EXPR41, EXPR42, EXPR43, EXPR44, EXPR45, EXPR46, EXPR47, EXPR48, EXPR49, EXPR50, 
    EXPR51, EXPR52, EXPR53, EXPR54, EXPR55, EXPR56, EXPR57, EXPR58, EXPR59, EXPR60, 
    EXPR61, EXPR62, EXPR63, EXPR64, EXPR65, EXPR66, EXPR67, EXPR68, EXPR69, EXPR70, 
    EXPR71, EXPR72, EXPR73, EXPR74, EXPR75, EXPR76, EXPR77, EXPR78, EXPR79, EXPR80, 
    EXPR81, EXPR82, EXPR83, EXPR84, EXPR85, EXPR86, EXPR87, EXPR88, EXPR89, EXPR90, 
    EXPR91, EXPR92, EXPR93, EXPR94, EXPR95, EXPR96, EXPR97, EXPR98, EXPR99, EXPR100
//I don't know why copilot need so many states, just keep it here
};

inline constexpr FSM token_type(TOKEN tk){
    return FSM::END;
}


}//namespace vastina

#endif
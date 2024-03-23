#ifndef _VASTINA_GLOBAL_DEFINE_
#define _VASTINA_GLOBAL_DEFINE_

namespace vastina{

enum TOKEN{
    UNKNOW = -1,
    UNSIGNED, SIGNED, INT, FLOAT, DOUBLE, BOOL, CHAR, VOID,
    VAR,
    NUMBER,
    NLBRAC, NRBRAC, OBRACE, CBRACE,
// 左小括号          左花括号
    MAIN, RETURN,
    SEMICOLON,  //分号
    GREATER, LESS, NOT, LOGNOT, AND, LOGAND, OR, LOGOR,
    NEG, ADD, MULTI, DIV, 
    IF, ELSE, FOR, WHILE, DO, SWITCH, CASE, BREAK, CONTINUE, DEFAULT,
    ASSIGN, EQUAL, NOTEQUAL, GREATEREQUAL, LESSEQUAL,//should these two?

    LINE, FILE,



    ASM//maybe in the future
};


enum FSM{//finite state machine

START,

END

};

inline constexpr FSM token_type(TOKEN tk){
    return FSM::END;
}


}//namespace vastina

#endif
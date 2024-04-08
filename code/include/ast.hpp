#ifndef _AST_H_
#define _AST_H_

namespace vastina{

class AstNode{
public:
    virtual ~AstNode() = default;
    virtual void print() = 0;
};



}


#endif
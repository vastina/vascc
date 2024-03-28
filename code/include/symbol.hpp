#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include <string_view>

namespace vastina{

template<typename ty>
class Literal{//compile time values like "Hello World",123,3.14
private:
    ty value;
public:
    Literal() = default;
    ~Literal() = default;
};

template<typename ty>
class Variable{
private:
    ty value;
    std::string_view name;

public:
    Variable(ty v,std::string_view n): value(v),name(n){};
    
    bool isSameName(std::string_view name);
    const ty& getValue();
};

typedef struct SymbolTable{

} SymbolTable;


}//namespace vastina


#endif
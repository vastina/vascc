#include "symbol.hpp"

namespace vastina{

template<typename  ty>
bool Variable<ty>::isSameName(std::string_view name){
    return this->name == name;
}

template<typename ty>
const ty& Variable<ty>::getValue(){
    return value;
}

} // namespace vastina

#ifndef _BASE_STRING_H_
#define _BASE_STRING_H_

#include <string>

#include "base/vasdef.hpp"

namespace vastina{
    
enum class CHARTYPE{
    CHAR,
    NUM,
    OTHER
};

inline constexpr CHARTYPE CharType(char ch){
    if(ch>='0' && ch<='9') return CHARTYPE::NUM;
    if((ch>='a' && ch<='z')||(ch=='_')||((ch>='A' && ch<='Z'))) return CHARTYPE::CHAR;
    return CHARTYPE::OTHER;
}
inline constexpr bool isWhiteSpace(char ch){
    return (ch==' '||ch=='\t'||ch=='\n'||ch=='\r');
}

inline bool Strcmp(const std::string& buffer, unsigned offset, const std::string_view& temp){
    unsigned len = temp.size();
    for(unsigned i=0; i<len; i++){
        if(buffer.at(i+offset) != temp.at(i))
            return false;
    }
    return true;
}

inline constexpr std::string_view _TypeName(std::string_view sv){
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

template<typename ty>
std::string_view ToString(const ty& t);

} // namespace vastina


#endif
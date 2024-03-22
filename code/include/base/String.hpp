#ifndef _STRING_H_
#define _STRING_H_

#include <string>

namespace vastina{
    
enum CHARTYPE{
    CHAR,
    NUM,
    OTHER
};

inline constexpr CHARTYPE CharType(char ch){
    if(ch>='0' && ch<='9') return CHARTYPE::NUM;
    if(ch>='a' && ch<='z') return CHARTYPE::CHAR;
    return CHARTYPE::OTHER;
}
inline constexpr bool isWhiteSpace(char ch){
    return (ch==' '||ch=='\t'||ch=='\n'||ch=='\r');
}

inline bool Strcmp(const std::string buffer, unsigned offset, const std::string temp){
    unsigned len = temp.size();
    for(unsigned i=0; i<len; i++){
        if(buffer.at(i+offset) != temp.at(i))
            return false;
    }
    return true;
}


} // namespace vastina


#endif
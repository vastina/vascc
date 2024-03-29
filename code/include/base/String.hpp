#ifndef _STRING_H_
#define _STRING_H_

#include <string>


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

inline bool Strcmp(const std::string& buffer, unsigned offset, const std::string& temp){
    unsigned len = temp.size();
    for(unsigned i=0; i<len; i++){
        if(buffer.at(i+offset) != temp.at(i))
            return false;
    }
    return true;
}


template<typename ty>
std::string_view ToString(const ty& t);

} // namespace vastina


#endif
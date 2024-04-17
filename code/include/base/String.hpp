#ifndef _BASE_STRING_H_
#define _BASE_STRING_H_

#include "base/vasdef.hpp"

#include <format>
#include <string_view>

namespace vastina {

enum class CHARTYPE {
    CHAR,
    NUM,
    OTHER
};

inline constexpr CHARTYPE
CharType(char ch) {
    if (ch >= '0' && ch <= '9')
        return CHARTYPE::NUM;
    if ((ch >= 'a' && ch <= 'z') || (ch == '_') || (ch >= 'A' && ch <= 'Z'))
        return CHARTYPE::CHAR;
    return CHARTYPE::OTHER;
}
inline constexpr bool
isWhiteSpace(char ch) {
    return (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r');
}

inline bool
Strcmp(const std::string &buffer, u32 offset,
       const std::string_view &temp) {
    u32 len = temp.size();
    for (u32 i = 0; i < len; i++) {
        if (buffer.at(i + offset) != temp.at(i))
            return false;
    }
    return true;
}

inline constexpr TOKEN
_TypeToken(std::string_view sv) {
    if (sv.size() == 0)
        return TOKEN::UNKNOW;

    switch (sv[0]) {
    case 'i':
        return TOKEN::INT;
        // return "int";
    case 'j':
        return TOKEN::UNSIGNED;
        // return "unsigned";
    case 'd':
        return TOKEN::DOUBLE;
        // return "double";
    case 'c':
        return TOKEN::CHAR;
        // return "char";
    case 'b':
        return TOKEN::BOOL;
        // return "bool";
    case 'f':
        return TOKEN::FLOAT;
        // retur "float";
    case 'l':
        return TOKEN::LONG;
        // return "long";
    case 'v':
        return TOKEN::VOID;
        // return "void";

    default:
        return TOKEN::UNKNOW;
    }
}

template <typename ty>
inline constexpr TOKEN
Type() {
    return _TypeToken(typeid(ty).name());
}

template <typename... Args>
void print(const std::string_view fmt_str, Args &&...args) {
    // auto fmt_args{ std::make_format_args(args...) };
    // std::string outstr{ std::vformat(fmt_str, fmt_args) };
    fputs(std::vformat(fmt_str, std::make_format_args(args...)).c_str(), stdout);
};

// auth: https://www.zhihu.com/people/guyutongxue
// link: https://zhuanlan.zhihu.com/p/688324926
// helps when transform enum to string
//  namespace detail {
//  template <auto V>
//  constexpr auto print() {
//      return std::string_view(std::source_location::current().function_name());
//  }
//  consteval auto find_name_pos() {
//      const auto result = print<nullptr>();
//      const auto left_omitted = result.find("nullptr");
//      const auto right_omitted = result.size() - left_omitted - 7;
//      return std::pair(left_omitted, right_omitted);
//  }
//  }
//  template <auto V>
//      requires (std::is_enum_v<decltype(V)>)
//  consteval auto enum_name() {
//      const auto function_str = detail::print<V>();
//      const auto [l, r] = detail::find_name_pos();
//      const auto value_str = function_str.substr(l, function_str.size() - l - r);
//      if (const auto scope_idx = value_str.rfind("::");
//          scope_idx != std::string_view::npos) {
//          return value_str.substr(scope_idx + 2);
//      } else {
//          return value_str;
//      }
//  }

// consteval std::unordered_map<TOKEN, std::string_view> TOKEN_STR;
// typedef struct before_main_t{
//     before_main_t(){
//         for(i32 i = UNKNOW; i < ASM; i++){
//             TOKEN_STR.insert(std::make_pair(static_cast<TOKEN>(i),
//                 enum_name<static_cast<TOKEN>(i)>()));
//         }
//     }
//     ~before_main_t() = default;
// } before_main_t;

} // namespace vastina

#endif
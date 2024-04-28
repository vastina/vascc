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
       const string_view &temp) {
    u32 len = temp.size();
    for (u32 i = 0; i < len; i++) {
        if (buffer.at(i + offset) != temp.at(i))
            return false;
    }
    return true;
}

inline const_str_t
p_token_str(P_TOKEN ptk) {
    switch (ptk) {
    // case P_TOKEN::CAL:
    //     return "calculate";
    case P_TOKEN::BINARY:
        return "binary";
    case P_TOKEN::FDECL:
        return "func declare";
    case P_TOKEN::VDECL:
        return "var declare";
    case P_TOKEN::ADDR:
        return "address";
    case P_TOKEN::IF:
        return "if";
    case P_TOKEN::LOOP:
        return "loop";
    case P_TOKEN::CALL:
        return "call";
    case P_TOKEN::RET:
        return "return";
    case P_TOKEN::END:
        return "end";
    default:
        return {};
    }
    return {};
}

inline constexpr TOKEN
_TypeToken(string_view sv) {
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
void print(const string_view fmt_str, Args &&...args) {
    // auto fmt_args{ std::make_format_args(args...) };
    // std::string outstr{ std::vformat(fmt_str, fmt_args) };
    fputs(std::vformat(fmt_str, std::make_format_args(args...)).c_str(), stdout);
};

template <typename... Args>
std::string format(const std::string_view &fmt_str, Args &&...args) {
    return std::vformat(fmt_str, std::make_format_args(args...));
}

// auth: https://www.zhihu.com/people/guyutongxue
// link: https://zhuanlan.zhihu.com/p/688324926
// helps when transform enum to string
//  namespace detail {
//  template <auto V>
//  constexpr auto print() {
//      return string_view(std::source_location::current().function_name());
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
//          scope_idx != string_view::npos) {
//          return value_str.substr(scope_idx + 2);
//      } else {
//          return value_str;
//      }
//  }
// consteval std::unordered_map<TOKEN, string_view> TOKEN_STR;
// typedef struct before_main_t{
//     before_main_t(){
//         for(i32 i = UNKNOW; i < ASM; i++){
//             TOKEN_STR.insert(std::make_pair(static_cast<TOKEN>(i),
//                 enum_name<static_cast<TOKEN>(i)>()));
//         }
//     }
//     ~before_main_t() = default;
// } before_main_t;

namespace x86 {

using vastina::format;

const_str_t file_start_{
    "\t.file\t\"{}\"\n"
    "\t.text\n"};
inline std::string file_start(const string_view &filename) {
    return format(file_start_, filename);
}

// {} {} ->func_name
const_str_t func_declare_start_{
    "\t.globl\t{}\n"
    "\t.type\t{}, @function\n"};
inline std::string func_declare_start(const string_view &func_name) {
    return format(func_declare_start_, func_name, func_name);
}

// {} -> counter, {} -> func_name
const_str_t func_start_{
    "{}:\n"
    ".LFB{}:\n"
    "\t.cfi_startproc\n"
    "\tendbr64\n"
    "\tpushq\t%rbp\n"
    "\tmovq\t%rsp, %rbp\n"};
inline std::string func_start(const string_view &func_name, u32 counter) {
    return format(func_start_, func_name, counter);
}

const_str_t func_end{
    "\tpopq\t%rbp\n"
    "\tret\n"
    "\t.cfi_endproc\n"};

// {} -> counter, {} {} -> func_name
const_str_t func_declare_end_{
    ".LFE{}:\n"
    "\t.size\t{}, .-{}\n"};
inline std::string func_declare_end(u32 counter, const string_view &func_name) {
    return format(func_declare_end_, counter, func_name, func_name);
}

const_str_t main_func_end{
    "\tleave\n"
    "\tret\n"
    "\t.cfi_endproc\n"};

// just keep it here
const_str_t asm_end_str{
    "\t.section\t.note.GNU-stack,\"\",@progbits\n"
    "\t.section\t.note.gnu.property,\"a\"\n"
    "\t.align 8\n"
    "\t.long\t1f - 0f\n"
    "\t.long\t4f - 1f\n"
    "\t.long\t5\n"
    "0:\n"
    "\t.string \"VAS\"\n"
    "1:\n"
    "\t.align 8\n"
    "\t.long\t0xc0000002\n"
    "\t.long\t3f - 2f\n"
    "2:\n"
    "\t.long\t0x3\n"
    "3:\n"
    "\t.align 8\n"
    "4:\n\n"};

// {} -> op, {},{} -> sth
const_str_t Threer_{
    "\t{}\t{}, {}\n"};
inline std::string Threer(const string_view &op, const string_view &_1, const string_view &_2) {
    return format(Threer_, op, _1, _2);
}

// {} -> op, {} -> sth
const_str_t Two_1{
    "\t{}\t{}\n"};
const_str_t Two_2{
    "\t{} {}\n"};
inline std::string Twoer_1(const string_view &op, const string_view &_1) {
    return format(Two_1, op, _1);
}
inline std::string Twoer_2(const string_view &op, const string_view &_1) {
    return format(Two_2, op, _1);
}

inline std::string make_call(const string_view &func_name) {
    return Twoer_1(call, func_name);
}

inline std::string make_jump(const string_view &op, const string_view &location) {
    return Twoer_1(op, location);
}

// {} -> value, {} -> register
const_str_t regIndirect_{
    "{}({})"};
inline std::string regIndirect(const string_view &val, const string_view &reg) {
    return format(regIndirect_, val, reg);
}

// {} -> counter, {} -> type, {} -> data
const_str_t rodata_{
    "\t.section\t.rodata\n"
    ".LC{}:\n"
    "\t.{} {}\n"
    "\t.text\n"};
inline std::string rodata(u32 counter, const string_view &type, const string_view &data) {
    return format(rodata_, counter, type, data);
}

const_str_t single_{
    "\t{}\n"};
inline std::string single(const string_view &op) {
    return format(single_, op);
}

// ${val}(${reg1},${reg2},${time})
const_str_t lea_er_{
    "{}({},{},{})"};
inline std::string leaer(const string_view &val, const string_view &reg1, const string_view &reg2, const string_view &time){
    return format(lea_er_, val, reg1, reg2, time);
}

const_str_t to_neg_{
    "\tnot\t{}\n"
    "\tinc\t{}\n"};
inline std::string to_neg(const string_view &reg) {
    return format(to_neg_, reg, reg);
}


}; // namespace x86


} // namespace vastina

#endif
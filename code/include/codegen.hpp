#ifndef _CODE_GENERATOR_H_
#define _CODE_GENERATOR_H_

#include "base/vasdef.hpp"
#include "base/String.hpp"

namespace vastina {

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

//just keep it here
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
   "4:\n\n"
    };


// {} -> op, {},{} -> sth
const_str_t Threer_{
    "\t{}\t{}, {}\n"};
inline std::string Threer(const string_view &op, const string_view &_1, const string_view &_2){
    return format(Threer_, op, _1, _2);
}

// {} -> op, {} -> sth
const_str_t Two_1{
    "\t{}\t{}\n"};
const_str_t Two_2{
    "\t{} {}\n"};
inline std::string Twoer_1(const string_view &op, const string_view &_1){
    return format(Two_1, op, _1);
}
inline std::string Twoer_2(const string_view &op, const string_view &_1){
    return format(Two_2, op, _1);
}

inline std::string make_call(const string_view &func_name) {
    return Twoer_1(call, func_name);
}

inline std::string make_jump(const string_view &op, const string_view &location){
    return Twoer_2(op, location);
}

// {} -> value, {} -> register
const_str_t regIndirect_{
    "{}({})"};
inline std::string regIndirect(const string_view& val, const string_view& reg){
    return format(regIndirect_, val, reg);
}

// {} -> counter, {} -> type, {} -> data
const_str_t rodata_{
    "\t.section\t.rodata\n"
    ".LC{}:\n"
    "\t.{} {}\n"
    "\t.text\n"};
inline std::string rodata(u32 counter, const string_view& type, const string_view& data){
    return format(rodata_, counter, type, data);
}

}; // namespace x86

}; // namespace vastina

#endif
#ifndef _CODE_GENERATOR_H_
#define _CODE_GENERATOR_H_

#include "base/String.hpp"
#include "base/vasdef.hpp"

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
    "\t.global\t{}\n"
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

}; // namespace x86

}; // namespace vastina

#endif
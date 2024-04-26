#include "base/io.hpp"
#include "codegen.hpp"

using namespace vastina;
int main() {
    (void)system("touch hello.s");
    auto writer{new Writer("hello.s")};

    writer->PushBack(x86::file_start("hello"));
    writer->PushBack(x86::func_declare_start("main"));
    writer->PushBack(x86::func_start("main", 0));
    writer->PushBack(std::string("\tmovl\t$0, %eax\n"));
    writer->PushBack(x86::main_func_end);
    writer->PushBack(x86::func_declare_end(0, "main"));
    writer->PushBack(x86::asm_end_str);

    writer->Open();
    writer->WriteAll();
    writer->Close();

    delete writer;
    return 0;
}
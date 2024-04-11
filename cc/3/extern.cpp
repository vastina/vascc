#include <iostream>

namespace sss {
    int aaa=10;
}

extern int _ZN3sss3aaaE;//the name of sss::aaa in symbol table

int main() {
    _ZN3sss3aaaE = 100;

    std::cout << sss::aaa <<'\n';

    return 0;
}
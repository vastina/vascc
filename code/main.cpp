#include "base/String.hpp"
#include "lexer.hpp"
#include "stmt.hpp"
#include "symbol.hpp"

#include <iostream>
#include <memory>
#include <queue>
#include <type_traits>

// const static before_main_t before_main = before_main_t();

using namespace vastina;
int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    lexer lx = lexer(argv[1]);
    std::cout << "--------------------------lexer-----------------------------"
                 "--------\n";
    while (lexer::STATE::END != lx.Next())
        ;
    auto tks = std::make_unique<std::vector<token_t>>(lx.getTokens());
    for (unsigned i = 0; i < tks->size(); i++) {
        print("offset:{}, \ttoken:{}, \tline:{}\n",
              i, tks->at(i).name, tks->at(i).line);
    }
    std::cout << "--------------------------preprocess------------------------"
                 "--------\n";
    Preprocess *pp = new Preprocess(*tks, lx.getScope());
    pp->Process();

    for (unsigned i = 0; i < pp->getSize(); i++) {
        auto &next = pp->getNext();
        print("offset: {}\nProcessedTokenType: {}\nstr:\t\"", i, Preprocess::p_token_str(next.tk));
        for (unsigned j = next.start; j < next.end; j++) {
            std::cout << tks->at(j).name << ' ';
        }
        std::cout << "\"\n";
    }
    std::cout << "--------------------------preprocess-result-----------------"
                 "--------\n";

    std::queue<decltype(pp->CurrentScope())> st;
    st.push(pp->CurrentScope());

    while (!st.empty()) {
        auto scope = st.front();
        st.pop();
        auto table = scope->getSymbolTable();
        print("start:\t {}\nend:\t {}\nvars: ", scope->getRange().start, scope->getRange().end);
        for (auto &&var : table.Variables)
            print("{}, ", var.first);
        print("\nfuns: ");
        for (auto &&fc : table.functions)
            print("{}, ", fc.first);
        print("\n");
        for (auto &&child : scope->getChildren())
            st.push(child);
    }

    std::cout << "------------------------------------------------------------"
                 "--------\n";

    // std::string filename{argv[1]};
    // filename += ".s";

    return 0;
}
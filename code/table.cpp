#include "ast.hpp"

namespace vastina{

before_main_t::~before_main_t(){};

before_main_t::before_main_t(){
    auto& mt = MatchTable::getInstance();
    mt.RegisteCase(TOKEN::INT, TOKEN::MAIN, FSM::DECL);
    // more todo

    mt.RegisteCase(TOKEN::INT, TOKEN::VAR, FSM::DECL);
}



}
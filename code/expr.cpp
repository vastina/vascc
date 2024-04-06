#include "expr.hpp"
#include "base/vasdef.hpp"

#include <iostream>

namespace vastina{

/*----------------------------------matchtable----------------------------------------------*/

// bool MatchTable::MatchUnit::operator==(const MatchUnit& m) const{
//     return tk_type == m.tk_type && current_state == m.current_state;
// }

// bool MatchTable::MatchUnit::operator!=(const MatchUnit& m) const{
//     return !(this->operator==(m));
// }

// MatchTable::MatchTable() = default;
// MatchTable::~MatchTable(){};

// void MatchTable::RegisteCase(TOKEN current_token, TOKEN except_token, FSM state){
//     auto& matcher = matchTable[current_token];
//     matcher.units.push_back({except_token, state});
// }

// void MatchTable::UnregisteCase(TOKEN current_token, TOKEN except_token, FSM state){
//     auto& matcher = matchTable[current_token];
//     for(auto it = matcher.units.begin(); it != matcher.units.end(); it++){
//         if(it->tk_type == except_token && it->current_state == state){
//             matcher.units.erase(it);
//             return;
//         }
//     }
// }

// void MatchTable::Error(const char* what){
//     std::cerr << what << std::endl;
//     exit(1);
// }//maybe I should use some logger

// void MatchTable::Warn(const char* what){
//     std::cerr << what << std::endl;
// }


// const std::vector<MatchTable::MatchUnit>& MatchTable::getMatchList(TOKEN tk){
//     return matchTable[tk].units;
// }

// TOKEN MatchTable::getNextMarch(TOKEN current_token){
//     auto& matcher = matchTable[current_token];
//     if(matcher.offset >= matcher.units.size()){
//         Warn("no match found");
//         return TOKEN::UNKNOW;
//     }
//     return matcher.units[matcher.offset++].tk_type;
// }

/*----------------------------------matchtable----------------------------------------------*/



} //namespace vastina
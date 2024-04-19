#ifndef _LOGGER_H_
#define _LOGGER_H_

//to avoid this:
// #include "base/log.hpp"
// #include <iostream>
#include <iostream>

namespace vastina {

// todo a log class, singlton or sth
#define RETURN_ERROR                                        \
    {                                                       \
        ::std::cerr << __FILE__ << ' ' << __LINE__ << '\n'; \
        return -1;                                          \
    }

#define EXIT_ERROR                                          \
    {                                                       \
        ::std::cerr << __FILE__ << ' ' << __LINE__ << '\n'; \
        exit(-1);                                           \
    }

#define THIS_NOT_SUPPORT(sth)                               \
    {                                                       \
        ::std::cerr << sth << " not supported now\n";       \
        ::std::cerr << __FILE__ << ' ' << __LINE__ << '\n'; \
        exit(-1);                                           \
    }

#define RETURN_NULL                                         \
    {                                                       \
        ::std::cerr << __FILE__ << ' ' << __LINE__ << '\n'; \
        return nullptr;                                     \
    }

#define tryCall(target, callee, ...)         \
    {                                        \
        if (callee(__VA_ARGS__) != target) { \
            EXIT_ERROR;                      \
        }                                    \
    }

#define EXCEPT_ZERO(callee, ...) tryCall(0, callee, __VA_ARGS__)

// make sure your interface named Next() and Peek()
//  last==true means if this is last case and don't match, stop and return error
#define Except(excepted, last, res) \
    do {                            \
        if (Peek() != excepted) {   \
            if (last) {             \
                EXIT_ERROR          \
            } else {                \
                res = -1;           \
            }                       \
        } else {                    \
            res = 0;                \
        }                           \
    } while (0);

#define tryNext(excepted, last)                                     \
    do {                                                            \
        Except(excepted, last, result) if (0 == result) { Next(); } \
        else if (last) {                                            \
            RETURN_ERROR                                            \
        }                                                           \
    } while (0)

#define trySkip(excepted, last)                           \
    do {                                                  \
        Except(excepted, last, result) if (0 == result) { \
            Next();                                       \
            Next();                                       \
        }                                                 \
        else if (last) {                                  \
            RETURN_ERROR                                  \
        }                                                 \
    } while (0)

// template<class fn, class... Args>
// auto tryCall(fn&& func, Args&& ...){
// };

} // namespace vastina

#endif
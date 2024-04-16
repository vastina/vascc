#ifndef _LOGGER_H_
#define _LOGGER_H_

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

// template<class fn, class... Args>
// auto tryCall(fn&& func, Args&& ...){
// };

} // namespace vastina

#endif
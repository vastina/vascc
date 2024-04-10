#ifndef _LOGGER_H_
#define _LOGGER_H_

namespace vastina {

// todo a log class, singlton or sth
#define RETURN_ERROR                                    \
    ::std::cerr << __FILE__ << ' ' << __LINE__ << '\n'; \
    return -1;

#define EXIT_ERROR                                      \
    ::std::cerr << __FILE__ << ' ' << __LINE__ << '\n'; \
    exit(-1);

#define THIS_NOT_SUPPORT(sth)                           \
    ::std::cerr << sth << " not supported now\n";       \
    ::std::cerr << __FILE__ << ' ' << __LINE__ << '\n'; \
    exit(-1);

#define RETURN_NULL                                   \
    std::cerr << __FILE__ << ' ' << __LINE__ << '\n'; \
    return nullptr;

} // namespace vastina

#endif
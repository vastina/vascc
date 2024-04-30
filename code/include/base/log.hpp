#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "base/String.hpp"

namespace vastina {

// todo a log class, singlton or sth

#define LEAVE_MSG( msg ) ::vastina::print( "{}, at {} {}\n", msg, __FILE__, __LINE__ );
#define LEAVE_NOTHING ::vastina::print( "{} {}\n", __FILE__, __LINE__ );

#define RETURN_ERROR                                                                                               \
  {                                                                                                                \
    LEAVE_NOTHING                                                                                                  \
    return -1;                                                                                                     \
  }

#define EXIT_ERROR                                                                                                 \
  {                                                                                                                \
    LEAVE_NOTHING                                                                                                  \
    exit( -1 );                                                                                                    \
  }

#define THIS_NOT_SUPPORT( sth )                                                                                    \
  {                                                                                                                \
    print( sth );                                                                                                  \
    LEAVE_MSG( " this not suppored now" )                                                                          \
    exit( -1 );                                                                                                    \
  }

#define RETURN_NULL                                                                                                \
  {                                                                                                                \
    LEAVE_NOTHING                                                                                                  \
    return nullptr;                                                                                                \
  }

#define tryCall( target, callee, ... )                                                                             \
  {                                                                                                                \
    if ( callee( __VA_ARGS__ ) != target ) {                                                                       \
      EXIT_ERROR;                                                                                                  \
    }                                                                                                              \
  }

#define EXCEPT_ZERO( callee, ... ) tryCall( 0, callee, __VA_ARGS__ )

// make sure your interface named Next() and Peek()
//  last==true means if this is last case and don't match, stop and return error
#define Except( excepted, last, res )                                                                              \
  {                                                                                                                \
    if ( Peek() != excepted ) {                                                                                    \
      if ( last ) {                                                                                                \
        EXIT_ERROR                                                                                                 \
      } else {                                                                                                     \
        res = -1;                                                                                                  \
      }                                                                                                            \
    } else {                                                                                                       \
      res = 0;                                                                                                     \
    }                                                                                                              \
  }

#define tryNext( excepted, last )                                                                                  \
  {                                                                                                                \
    Except( excepted, last, result ) if ( 0 == result )                                                            \
    {                                                                                                              \
      Next();                                                                                                      \
    }                                                                                                              \
    else if ( last )                                                                                               \
    {                                                                                                              \
      RETURN_ERROR                                                                                                 \
    }                                                                                                              \
  }

#define trySkip( excepted, last )                                                                                  \
  {                                                                                                                \
    Except( excepted, last, result ) if ( 0 == result )                                                            \
    {                                                                                                              \
      Next();                                                                                                      \
      Next();                                                                                                      \
    }                                                                                                              \
    else if ( last )                                                                                               \
    {                                                                                                              \
      RETURN_ERROR                                                                                                 \
    }                                                                                                              \
  }

// template<class fn, class... Args>
// auto tryCall(fn&& func, Args&& ...){
// };

} // namespace vastina

#endif
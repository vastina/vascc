#ifndef _VAS_IO_H_
#define _VAS_IO_H_

#include <deque>
#include <string>

namespace vastina {

class Buffer {
  protected:
    std::deque<std::string> buffer_queue_;
};

class Writer {
  protected:
    ;

  public:
    void WriteLine(/*args*/);
};

} // namespace vastina

#endif
#ifndef _VAS_IO_H_
#define _VAS_IO_H_

#include "base/vasdef.hpp"
#include <deque>
#include <fstream>
#include <string>

namespace vastina {

class Buffer
{
protected:
  std::deque<std::string> buffer_queue_;

public:
  Buffer();
  ~Buffer();
  u32 PushBack( const std::string& );
  u32 PushBack( const string_view& );
  void PushFront( const std::string& );
  void Insert( u32, const std::string& );
  std::string& Peekat( u32 );
  std::string PopBack();
  std::string PopFront();
};

class Writer : public Buffer
{
protected:
  std::string file_name_;
  std::fstream fs_;

public:
  using pointer = Writer*;
  Writer( const string_view& file_name );
  ~Writer();
  void Open();
  void Close();
  void WriteOne();
  void WriteAll();
};

} // namespace vastina

#endif // _VAS_IO_H_

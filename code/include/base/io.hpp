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

  bool _M_check( u32 __n ) const;
  std::string& _M_at( u32 __n );

public:
  Buffer();
  virtual ~Buffer();
  u32 PushBack( const std::string& str );
  u32 PushBack( const string_view& str );
  u32 PushBack( const char* str );
  void PushFront( const std::string& str );
  void Override( u32 pos, const std::string& str );
  void Insert( u32 pos, const std::string& str );
  std::string& Peekat( u32 pos );
  std::string PopBack();
  std::string PopFront();
};

class Reader;
class Writer;

class Filer : public Buffer
{
public:
  using pointer = Filer*;

protected:
  std::string file_name_;
  std::fstream fs_;

public:
  Filer( const string_view& file_name );
  virtual ~Filer();

  const Reader& reader() const;
  const Writer& writer() const;
  Reader& reader();
  Writer& writer();

  void Open();
  void Close();
};

class Writer : public Filer
{
public:
  Writer( const string_view& file_name );
  ~Writer();
  void WriteOne();
  void WriteAll();
};

class Reader : public Filer
{
public:
  Reader( const string_view& file_name );
  ~Reader();
  void ReadOne();
  void Readline();
  void ReadAll();
};

} // namespace vastina

#endif // _VAS_IO_H_
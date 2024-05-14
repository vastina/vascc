#include "base/io.hpp"
#include <deque>
#include <fstream>
#include <string>

namespace vastina {

// Implementation of Buffer class
Buffer::Buffer() : buffer_queue_ {} {}
Buffer::~Buffer()
{
  buffer_queue_.clear();
}

u32 Buffer::PushBack( const std::string& str )
{
  buffer_queue_.push_back( str );
  return buffer_queue_.size() - 1;
}

u32 Buffer::PushBack( const string_view& str )
{
  std::string s { str };
  return PushBack( s );
}

void Buffer::PushFront( const std::string& str )
{
  buffer_queue_.push_front( str );
}

void Buffer::Insert( u32 pos, const std::string& str )
{
  buffer_queue_.erase( buffer_queue_.begin() + pos );
  buffer_queue_.insert( buffer_queue_.begin() + pos, str );
}

std::string& Buffer::Peekat( u32 pos )
{
  return buffer_queue_.at( pos );
}

std::string Buffer::PopBack()
{
  auto str = buffer_queue_.at( buffer_queue_.size() - 1 );
  buffer_queue_.pop_back();
  return str;
}

std::string Buffer::PopFront()
{
  auto str = buffer_queue_.at( 0 );
  buffer_queue_.pop_front();
  return str;
}

// Implementation of Writer class
Writer::Writer( const string_view& file_name ) : Buffer(), file_name_( file_name ), fs_ {} {}
Writer::~Writer()
{
  if ( fs_.is_open() )
    fs_.close();
  file_name_.clear();
}

void Writer::Open()
{
  fs_.open( file_name_ );
}

void Writer::Close()
{
  fs_.close();
}

void Writer::WriteOne()
{
  fs_ << PopFront();
}

void Writer::WriteAll()
{
  while ( !buffer_queue_.empty() ) {
    WriteOne();
  }
}

} // namespace vastina

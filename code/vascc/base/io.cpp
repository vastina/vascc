#include "base/io.hpp"
#include <deque>
#include <fstream>
#include <string>

namespace vastina {

static_assert( sizeof( Writer ) == sizeof( Filer ) );
static_assert( sizeof( Reader ) == sizeof( Filer ) );

const Writer& Filer::writer() const
{
  return static_cast<const Writer&>( *this );
}

const Reader& Filer::reader() const
{
  return static_cast<const Reader&>( *this );
}

Writer& Filer::writer()
{
  return static_cast<Writer&>( *this );
}

Reader& Filer::reader()
{
  return static_cast<Reader&>( *this );
}

Buffer::Buffer() : buffer_queue_ {} {}
Buffer::~Buffer()
{
  buffer_queue_.clear();
}

bool Buffer::_M_check( u32 __n ) const
{
  return __n < buffer_queue_.size();
}

std::string& Buffer::_M_at( u32 __n )
{
  if ( _M_check( __n ) )
    return buffer_queue_.at( __n );
  throw std::out_of_range( "Buffer::_M_at: index out of range" );
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

u32 Buffer::PushBack( const char* str )
{
  std::string s { str };
  return PushBack( s );
}

void Buffer::PushFront( const std::string& str )
{
  buffer_queue_.push_front( str );
}

void Buffer::Override( u32 pos, const std::string& str )
{
  buffer_queue_.erase( buffer_queue_.begin() + pos );
  buffer_queue_.insert( buffer_queue_.begin() + pos, str );
}

void Buffer::Insert( u32 pos, const std::string& str )
{
  buffer_queue_.insert( buffer_queue_.begin() + pos, str );
}

std::string& Buffer::Peekat( u32 pos )
{
  return _M_at( pos );
}

std::string Buffer::PopBack()
{
  auto str = _M_at( buffer_queue_.size() - 1 );
  buffer_queue_.pop_back();
  return str;
}

std::string Buffer::PopFront()
{
  auto str = _M_at( 0 );
  buffer_queue_.pop_front();
  return str;
}

// Filer class member functions
Filer::Filer( const string_view& file_name ) : Buffer(), file_name_( file_name ), fs_ {} {}

Filer::~Filer()
{
  if ( fs_.is_open() )
    fs_.close();
  file_name_.clear();
}

void Filer::Open()
{
  fs_.open( file_name_ );
}

void Filer::Close()
{
  fs_.close();
}

// Writer class member functions
Writer::Writer( const string_view& file_name ) : Filer( file_name ) {}

Writer::~Writer() {}

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

// Reader class member functions
Reader::Reader( const string_view& file_name ) : Filer( file_name ) {}

Reader::~Reader() {}

void Reader::ReadOne()
{
  std::string str {};
  fs_ >> str;
  PushBack( str );
}

void Reader::Readline()
{
  std::string str {};
  std::getline( fs_, str );
  PushBack( str.append( "\n" ) );
}

void Reader::ReadAll()
{
  while ( !fs_.eof() ) {
    ReadOne();
  }
}

} // namespace vastina

#include "symbol.hpp"
#include "base/log.hpp"

namespace vastina {

// get the child just created so add data in it
Scope::pointer Scope::CreateChild( range_t&& rr = { 0, 0 } )
{
  children_.push_back( new Scope( this, std::move( rr ) ) );
  return children_.back();
}
Scope::pointer Scope::getParent()
{
  return parent_;
}
Scope::pointer Scope::getRoot()
{
  auto root = this;
  while ( nullptr != root->parent_ ) {
    root = root->parent_;
  }
  return root;
}
void Scope::addVar( const string_view& name, Variable::pointer var )
{
  st_->addVar( name, var );
}
void Scope::addFunc( const string_view& name, Function::pointer fc )
{
  st_->addFunc( name, fc );
}
Variable::pointer Scope::getVar( const string_view& name )
{
  auto node = this;
  do {
    auto res = node->st_->getVar( name );
    if ( nullptr != res )
      return res;
    node = node->parent_;
  } while ( nullptr != node );
  RETURN_NULL
}
Function::pointer Scope::getFunc( const string_view& name )
{
  auto node = this;
  do {
    auto res = node->st_->getFunc( name );
    if ( nullptr != res )
      return res;
    node = node->parent_;
  } while ( nullptr != node );
  RETURN_NULL
}
bool Scope::varExist( const string_view& name )
{
  auto node = this;
  do {
    if ( node->st_->varExist( name ) ) {
      return true;
    }
    node = node->parent_;
  } while ( nullptr != node );

  return false;
}

bool Scope::funcExist( const string_view& name )
{
  auto node = this;
  do {
    if ( node->st_->funcExist( name ) ) {
      return true;
    }
    node = node->parent_;
  } while ( nullptr != node );

  return false;
}

void Scope::setRange( u32 start, u32 end )
{
  r_.start = start;
  r_.end = end;
}

void Scope::setBreakable( bool breakable )
{
  isBreakable_ = breakable;
}
const range_t& Scope::getRange()
{
  return r_;
}
const decltype( Scope::children_ )& Scope::getChildren()
{
  return children_;
}

const SymbolTable::pointer& Scope::getSymbolTable()
{
  return st_;
}
// you can get other if you like
Scope::pointer Scope::getNextChild()
{
  return children_.at( idchild_++ );
}
Scope::pointer Scope::getChildat( u32 offst )
{
  return children_.at( offst );
}

void SymbolTable::useBuiltin()
{
  constexpr u32 unreachable_line { 0u };
  {
    const static token_t printf { TOKEN::SYMBOLF, string_view( "printf" ), unreachable_line };
    auto _builtin_printf { new Function( printf, TOKEN::INT ) };
    _builtin_printf->ty_.isBuiltin_ = true;
    _builtin_printf->ty_.isUseValist_ = true;
    //_builtin_printf->isVoid_ = false;
    addFunc( printf.name, _builtin_printf );
  }
  {
    const static token_t scanf { TOKEN::SYMBOLF, string_view( "scanf" ), unreachable_line };
    auto _builtin_scanf { new Function( scanf, TOKEN::INT ) };
    _builtin_scanf->ty_.isBuiltin_ = true;
    _builtin_scanf->ty_.isUseValist_ = true;
    //_builtin_printf->isVoid_ = false;
    addFunc( scanf.name, _builtin_scanf );
  }
}

} // namespace vastina

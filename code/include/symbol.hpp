#ifndef _SYMBOL_H_
#define _SYMBOL_H_

// so, this file contains information from lexer to asmCodeGen

#include "base/vasdef.hpp"

#include <memory>
#include <unordered_map>
#include <vector>
#include <functional>

namespace vastina {
struct token_t
{
  TOKEN token;
  string_view name;
  u32 line;

  // some were here just because I am lazy to delete them
  // well, all of them are in use 5/14/24
  token_t( TOKEN tk );
  token_t( TOKEN tk, const string_view& sv );
  token_t( TOKEN tk, string_view&& sv );
  token_t( TOKEN tk, const string_view& sv, u32 _line );
  token_t( TOKEN tk, string_view&& sv, u32 _line );

  token_t( const token_t& tk );
  token_t( token_t&& tk );
};

using SourceLocation = token_t;
typedef struct Location
{ //.LC${count}
  u32 count_;
} Location;
typedef struct sLoc
{ // stack location
  u32 offset_;
  bool isPointer_;
} sLocation;

class Value
{

protected:
  const SourceLocation& Srcloc_;
  TOKEN type_ {}; // so todo

public:
  using pointer = Value*;
  Value( const SourceLocation& srcloc, TOKEN type ) : Srcloc_( srcloc ), type_( type ) {}
  ~Value() = default;
  virtual const string_view& getName() const { return Srcloc_.name; }
  virtual const SourceLocation& getSrcloc() const { return Srcloc_; }

  inline pointer self() { return this; }
  inline TOKEN getType() { return type_; }
};

class literal : public Value
{ // compile time values like "Hello World",(114514,3.14) -> Trivial

protected:
  Location loc_;

public:
  literal( const SourceLocation& Srcloc ) : Value( Srcloc, {} ) {}
  literal( const SourceLocation& Srcloc, TOKEN type ) : Value( Srcloc, type ) {}
  ~literal() {}

  inline void setLocation() {}
  inline const Location& getLocation() { return loc_; }
};

class Variable : public Value
{
public:
  using pointer = Variable*;

protected:
  sLoc stack_location_;
  bool isConst_ {};
  bool isTrivial_ {};

public:
  Variable() = delete;
  Variable( const SourceLocation& srcloc, TOKEN type ) : Value( srcloc, type ) {}
  ~Variable() {}
  inline bool isConst() { return isConst_; }
};

class Function : public Value
{
protected:
  bool isVoid_ {};
  std::vector<Variable::pointer> paras_;

public:
  using pointer = Function*;

  Function( const SourceLocation& Srcloc ) : Value( Srcloc, {} ), paras_ {} {} // for lexer
  Function( const SourceLocation& Srcloc, TOKEN type ) : Value( Srcloc, type ), paras_ {} {}
  ~Function() { paras_.clear(); }

  virtual u32 getParamSize() { return paras_.size(); }
  void addPara( Variable::pointer var ) { paras_.push_back( var ); }
};

typedef struct SymbolTable
{
  using pointer = SymbolTable*;

  std::unordered_map<string_view, Variable::pointer>* Variables;
  std::unordered_map<string_view, Function::pointer>* functions;

  SymbolTable()
    : Variables { new std::unordered_map<string_view, Variable::pointer>() }
    , functions { new std::unordered_map<string_view, Function::pointer> }
  {}
  ~SymbolTable()
  {
    Variables->clear();
    functions->clear();
  }

  inline bool varExist( const string_view& name ) const
  {
    return Variables->contains( name );
    // return static_cast<bool>(Variables->count(name));
  }
  inline bool funcExist( const string_view& name ) const { return functions->contains( name ); }
  // Variables.insert(std::make_pair(name, var)); }
  inline Variable::pointer getVar( const string_view& name ) const
  {
    if ( varExist( name ) )
      return Variables->at( name );
    return nullptr;
  }
  inline Function::pointer getFunc( const string_view& name ) const
  {
    if ( funcExist( name ) )
      return functions->at( name );
    return nullptr;
  }

  // always override
  inline void addVar( const string_view& name, Variable::pointer var )
  {
    Variables->insert_or_assign( name, var );
    // Variables->erase( name );
    // Variables->insert( std::make_pair( name, var ) );
    // Variables[name] = var;
  }
  inline void addFunc( const string_view& name, Function::pointer fc ) { functions->insert_or_assign( name, fc ); }

  // inline Variable::pointer getVar( `the source-location` ) to-do
  // this to-do seems a little impossible now
} SymbolTable;

typedef struct TypeTable
{
} TypeTable;

typedef struct range_t
{
  u32 start;
  u32 end; //[start, end) preprocessed_tokens[start] to
           // preprocessed_tokens[end-1]

  // some are here just because I'm lazy to check which is no longer needed
  range_t() : start( 0 ), end( 0 ) {}
  inline const range_t& operator=( range_t&& other )
  {
    start = other.start;
    end = other.end;
    return *this;
  }
  range_t( u32 st, u32 ed ) : start( st ), end( ed ) {}
  range_t( range_t&& other ) : start( other.start ), end( other.end ) {}
  range_t( const range_t& other ) : start( other.start ), end( other.end ) {}

} range_t;

class Scope
{
public:
  using pointer = Scope*;
  using ScopePtr = std::shared_ptr<Scope>;

private:
  pointer parent_; // if can't find symbol, ask parent
  range_t r_;
  // "create symbol tables for every scope"
  SymbolTable::pointer st_; // so decl a samename-var in child scope is acceptable
  std::vector<pointer> children_;

  bool isBreakable_ { false }; // for loop, who else need this?

public:
  Scope() = delete;
  Scope( range_t&& r ) : parent_( nullptr ), r_( r ), st_( new SymbolTable() ), children_() {}
  Scope( pointer parent, range_t&& r ) : parent_( parent ), r_( r ), st_( new SymbolTable() ), children_() {}

  void addVar( const string_view&, Variable::pointer );
  void addFunc( const string_view&, Function::pointer );
  Variable::pointer getVar( const string_view& );
  Function::pointer getFunc( const string_view& );
  bool varExist( const string_view& );
  bool funcExist( const string_view& );

  void setRange( u32, u32 );
  const range_t& getRange();
  // range_t findRange(u32);
  // range_t getNextRangeBetweenChildren(); //this is too stupid, I won't do that
  void setBreakable( bool );
  inline void reSet()
  {
    idchild_ = 0;
    for ( auto& child : children_ )
      child->reSet();
  };

  // for test
  const decltype( children_ )& getChildren();
  const decltype( st_ )& getSymbolTable();

  pointer CreateChild( range_t&& );
  pointer getNextChild();
  pointer getChildat( u32 );
  pointer getParent();
  pointer getRoot();

  // so bad
  u32 idchild_ {};
};

typedef struct p_token_t
{
  P_TOKEN tk;
  u32 start;
  u32 end;

  inline void setRange( u32 _start, u32 _end )
  {
    start = _start;
    end = _end;
  }
} p_token_t;
//[start, end) -> primary_tokens_
// use rang_t here maybe better

class Preprocess
{
public:
  using pointer = Preprocess*;

private:
  std::vector<token_t>& primary_tokens;
  u32 offset;

  std::vector<p_token_t> results;
  u32 id = 0;

  Scope::pointer current_scope;

public:
  Preprocess( std::vector<token_t>& tks ) : primary_tokens( tks ), offset(), current_scope( new Scope( { 0, 0 } ) )
  {}
  Preprocess( std::vector<token_t>& tks, Scope::pointer self ) : primary_tokens( tks ), offset()
  {
    current_scope = self;
  };
  ~Preprocess() { results.clear(); };

private:
  // Getter and Setter----------------------------------------------------
  inline TOKEN Current();
  inline const token_t& CurrentToken();
  inline const string_view& CurrentTokenName();
  inline TOKEN Peek();
  void Next();
  void reset();

  void Backup( u32 );
  // Getter and Setter----------------------------------------------------
private:
  // for Except, use this when you need Except
  // and you can get a more friendly log, instead of cerr in Except(),
  // always the same line
  i32 result {};
  // someone need custom judge

  // return a code to indicate the result
  i32 Binary( const std::function<bool()>& EndJudge );
  i32 Assign( const std::function<bool()>& EndJudge );
  i32 Declare( const std::function<bool()>& EndJudge );
  i32 Address( const std::function<bool()>& EndJudge );
  i32 Ifer();
  i32 RetType();

  i32 Paras( Function::pointer fc );
  i32 Callee( Function::pointer callee );
  i32 FuncDecl();

  i32 LoopW(); // while
  i32 LoopF(); // for
  i32 LoopD(); // do

public:
  i32 Process();

  // Getter and Setter----------------------------------------------------
  const p_token_t& getNext();
  u32 getSize() const;

  const std::vector<p_token_t>& getResult();
  // for test?
  Scope::pointer CurrentScope();
  // Getter and Setter----------------------------------------------------
};

} // namespace vastina

#endif
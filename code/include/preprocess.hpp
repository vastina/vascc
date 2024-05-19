#include "symbol.hpp"

#include <functional>

namespace vastina {

// todo file preprocess

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
  const std::vector<token_t>& primary_tokens;
  u32 offset;

  std::vector<p_token_t> results;
  u32 id = 0;

  Scope::pointer current_scope;

public:
  Preprocess( const std::vector<token_t>& tks )
    : primary_tokens( tks ), offset(), current_scope( new Scope( { 0, 0 } ) )
  {}
  Preprocess( const std::vector<token_t>& tks, Scope::pointer self ) : primary_tokens( tks ), offset()
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

};

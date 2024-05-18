#include "preprocess.hpp"
#include "base/log.hpp"

#include <iostream>
#include <functional>

namespace vastina {

#define TEMP_LOG                                                                                                   \
  {                                                                                                                \
    ::std::cerr << offset << ' ' << Current() << ' ' << CurrentTokenName() << ' ';                                 \
  }

inline TOKEN Preprocess::Current()
{
  return primary_tokens[offset].token;
}

inline const token_t& Preprocess::CurrentToken()
{
  return primary_tokens[offset];
}

inline const string_view& Preprocess::CurrentTokenName()
{
  return primary_tokens[offset].name;
}

inline TOKEN Preprocess::Peek()
{
  return primary_tokens[offset + 1].token;
}

void Preprocess::Next()
{
  offset++;
}

void Preprocess::reset()
{
  offset = 0;
  id = 0;
}

void Preprocess::Backup( u32 pos )
{
  while ( results.size() >= pos )
    results.pop_back();
  return;
}

i32 Preprocess::Process()
{
  u32 size = primary_tokens.size();
  while ( offset < size ) {
    switch ( Current() ) {
      case TOKEN::SEMICOLON:
        results.push_back( { P_TOKEN::END, offset, offset + 1 } );
        Next();
        break;
      case TOKEN::OBRACE: {
        current_scope = current_scope->getNextChild();
        current_scope->setRange( getSize(), 0 );
        Next();
        break;
      }
      case TOKEN::CBRACE: {
        current_scope->setRange( current_scope->getRange().start, getSize() );
        current_scope = current_scope->getParent();
        Next();
        break;
      }
      case TOKEN::IF: {
        EXCEPT_ZERO( Ifer );
        break;
      }
      case TOKEN::SYMBOL: {
        if ( !current_scope->varExist( CurrentTokenName() ) ) {
          TEMP_LOG
          EXIT_ERROR
        }
        if ( token_type( Peek() ) == TOKEN_TYPE::OPERATOR ) {
          EXCEPT_ZERO( Binary, [this]() { return Current() == TOKEN::SEMICOLON || Current() == TOKEN::COMMA; } );
          break;
        } else {
          Except( TOKEN::SEMICOLON, true, result );
          results.push_back( { P_TOKEN::END, offset, offset + 1 } );
        }
        break;
      }
      case TOKEN::SYMBOLF: { // void or return value ignored go here
        if ( !current_scope->funcExist( CurrentTokenName() ) ) {
          TEMP_LOG
          EXIT_ERROR
        }
        //results.push_back( { P_TOKEN::CALL, offset, offset + 1 } );
        EXCEPT_ZERO( Callee, current_scope->getFunc( CurrentTokenName() ) );
        if(Current() == TOKEN::NRBRAC) Next();
        // if(Callee(current_scope->getFunc(CurrentTokenName()))!=0) EXIT_ERROR;
        break;
      }
      case TOKEN::WHILE:
        EXCEPT_ZERO( LoopW );
        break;
      case TOKEN::FOR:
        EXCEPT_ZERO( LoopF );
        break;
      case TOKEN::DO:
        EXCEPT_ZERO( LoopD );
        break;
      case TOKEN::FUNC:
        EXCEPT_ZERO( FuncDecl );
        break;
      case TOKEN::RETURN:
        EXCEPT_ZERO( RetType );
        break;
      default: {
        switch ( token_type( Current() ) ) {
          case TOKEN_TYPE::TYPE: {
            if ( Peek() == TOKEN::SYMBOL )
              EXCEPT_ZERO( Declare, [this]() { return Current() == TOKEN::SEMICOLON; } )
            else if ( Peek() == TOKEN::SYMBOLF || Peek() == TOKEN::MAIN )
              EXCEPT_ZERO( FuncDecl )
            else {
              print( "peek token: {}\n", (i32)Peek() );
              EXIT_ERROR;
            }
            break;
          }
          case TOKEN_TYPE::VALUE: {
            tryNext( TOKEN::SEMICOLON, true );
            results.push_back( { P_TOKEN::BINARY, offset - 1, offset } );
            results.push_back( { P_TOKEN::END, offset, offset + 1 } );
            break;
          }

          default:
            TEMP_LOG;
            EXIT_ERROR;
        }
      }
    }
  }

  // it should be the global scope now
  current_scope->setRange( 0, getSize() );
  // this is bad
  // 4/24/24, yeah, this is bad and cause a bug, I fix it today
  current_scope->reSet();
  return 0;
}

i32 Preprocess::Binary( const std::function<bool()>& EndJudge )
{
  typedef struct BracketCount
  {
    u32 open {};
    u32 close {};
  } BracketCount;

  BracketCount bc;
  u32 last_offset = offset;
  results.push_back( { P_TOKEN::BINARY, 0, 0 } );
  u32 __pos__ = results.size() - 1;

  while ( true ) {
    switch ( token_type( Current() ) ) {
      case TOKEN_TYPE::BRAC: {
        if ( Current() == TOKEN::NRBRAC ) {
          if ( ++bc.close > bc.open ) {
            TEMP_LOG
            RETURN_ERROR
          }
        } else if ( Current() == TOKEN::NLBRAC ) {
          ++bc.open;
        } else {
          TEMP_LOG
          RETURN_ERROR
        }
        break;
      }
      case TOKEN_TYPE::VALUE: {
        if ( token_type( Peek() ) == TOKEN_TYPE::VALUE ) {
          TEMP_LOG;
          RETURN_ERROR
        }

        switch ( Current() ) {
          case TOKEN::SYMBOL:
            if ( !current_scope->varExist( CurrentTokenName() ) ) {
              TEMP_LOG
              RETURN_ERROR
            }
            break;
          case TOKEN::SYMBOLF: // nonvoid go here
            if ( !current_scope->funcExist( CurrentTokenName() ) ) {
              TEMP_LOG
              RETURN_ERROR
            }
            EXCEPT_ZERO( Callee, current_scope->getFunc( CurrentTokenName() ) );
            break;
          case TOKEN::NUMBER:
            break;
          case TOKEN::STRING:
            break;
          case TOKEN::LCHAR:
          default:
            break;
        }
        break;
      }
      case TOKEN_TYPE::OPERATOR: {
        auto peek = Peek();
        if ( token_type( peek ) == TOKEN_TYPE::OPERATOR ) {
          if ( peek != TOKEN::ASSIGN && peek != TOKEN::NEG && peek != TOKEN::PLUS && peek != TOKEN::OPS
               && peek != TOKEN::LOGNOT ) {
            TEMP_LOG;
            RETURN_ERROR
          }
          if ( peek == TOKEN::ASSIGN && Current() == TOKEN::NOTEQUAL ) {
            TEMP_LOG;
            RETURN_ERROR;
          }
        }
        break;
      }
      default: {
        TEMP_LOG;
        RETURN_ERROR
      }
    }
    Next();
    if ( EndJudge() && ( bc.close == bc.open ) ) {
      // results.push_back({P_TOKEN::BINARY, last_offset, offset});
      results.at( __pos__ ).setRange( last_offset, offset );
      // if(Current() == TOKEN::SEMICOLON)
      //     results.push_back({P_TOKEN::END, offset, offset + 1});
      break;
    }
  }

  return 0;
}

i32 Preprocess::Declare( const std::function<bool()>& EndJudge )
{

  std::function<void()> adder;
  auto type { Current() };
  switch ( Current() ) {
    case TOKEN::INT:
    case TOKEN::FLOAT:
    case TOKEN::DOUBLE:
      adder = [this, type]() { current_scope->addVar( CurrentTokenName(), new Variable( CurrentToken(), type ) ); };
      break;
    default: {
      TEMP_LOG
      RETURN_ERROR
    }
  }
  Next();

  u32 last_offset = offset;
  while ( true ) {
    if ( Current() == TOKEN::SYMBOL ) {
      if ( Peek() != TOKEN::ASSIGN ) {
        results.push_back( { P_TOKEN::VDECL, offset, offset + 1 } );
      }
      adder();
      Next();
    } else if ( Current() == TOKEN::ASSIGN ) {
      results.push_back( { P_TOKEN::VDECL, last_offset, offset + 1 } );
      last_offset = offset + 1;
      Next();

      (void)Binary( [this]() { return Current() == TOKEN::SEMICOLON || Current() == TOKEN::COMMA; } );
    } else if ( EndJudge() )
      break;
    else if ( Current() == TOKEN::COMMA ) {
      last_offset = offset + 1;
      Next();
    }
  }

  return 0;
}

i32 Preprocess::Address( const std::function<bool()>& )
{
  return {};
}

i32 Preprocess::Ifer()
{
  results.push_back( { P_TOKEN::IF, offset, offset + 1 } );

  tryNext( TOKEN::NLBRAC, true );
  // EXCEPT_ZERO(Binary, fn);
  i32 res = Binary( [this]() { return Current() == TOKEN::OBRACE; } ); // 暂时不支持不带{}的if
  if ( 0 != res )
    RETURN_ERROR;

  return 0;
}

i32 Preprocess::Callee( Function::pointer func )
{
  results.push_back( { P_TOKEN::CALL, offset, offset + 1 } );
  // nothing done here actually
  trySkip( TOKEN::NLBRAC, true );
  if ( Current() == TOKEN::NRBRAC )
    return 0;

  u32 count = 0;
  while ( true ) {
    if ( Binary( [this] { return ( Current() == TOKEN::COMMA ) || ( Current() == TOKEN::NRBRAC ); } ) != 0 ) {
      RETURN_ERROR
    }
    if ( ++count > func->getParamSize() && !func->ty_.isUseValist_ )
      RETURN_ERROR;
    
    if ( Current() == TOKEN::NRBRAC )
      break;
    else
      Next();
  }

  return 0;
}

i32 Preprocess::Paras( Function::pointer func )
{
  // just kepp it like this, do not modify if you really know what you
  // are doing
  if ( Peek() == TOKEN::NRBRAC ) {
    Next();
    return 0;
  } else {
    Next();
  }
  while ( true ) {
    if ( Declare( [this, func, counter { 0u }]() mutable {
           if ( results.size() > counter ) {
             counter = results.size();
             func->addPara( current_scope->getVar( primary_tokens.at( results.at( counter - 1 ).start ).name ) );
           }
           return ( ( Current() == TOKEN::COMMA )
                    || ( ( Current() == TOKEN::NRBRAC )
                         && ( ( Peek() == TOKEN::SEMICOLON ) || ( Peek() == TOKEN::OBRACE ) ) ) );
         } )
         != 0 ) {
      RETURN_ERROR
    }
    if ( Current() != TOKEN::COMMA )
      break;
    else {
      Next();
    }
  }
  return 0;
}
i32 Preprocess::FuncDecl()
{
  auto last_offset = offset;
  TOKEN type = Current();
  auto flag { token_type( type ) == TOKEN_TYPE::TYPE };

  Next();
  auto&& func_token = CurrentToken();
  // 4/25/24 remove `&&` will cause a bug, you can't get func->getName() out of lambda or the scope(when add instead
  // of adder)

  // if (Current() == TOKEN::MAIN) {} // seems nothing to-do, error `undefined ref to _start` happen at ld time

  if ( !flag ) {
    trySkip( TOKEN::COLON, true );
    type = Current();
  }

  std::function<void()> adder;
  switch ( type ) {
    case TOKEN::INT:
    case TOKEN::FLOAT:
    case TOKEN::CHAR:
      adder = [this, &func_token]() {
        current_scope->addFunc( func_token.name, new Function( func_token, Current() ) );
      };
      break;
    default:
      THIS_NOT_SUPPORT( CurrentTokenName() );
  }
  adder(); // adder or just add?

  results.push_back( { P_TOKEN::FDECL, 0, 0 } );
  u32 __pos__ = results.size() - 1;

  tryNext( TOKEN::NLBRAC, true );
  current_scope = current_scope->getChildat( current_scope->idchild_ ); // getNextchild(0), jump in temply
  EXCEPT_ZERO( Paras, current_scope->getFunc( func_token.name ) );
  current_scope = current_scope->getParent();
  tryNext( TOKEN::OBRACE, false );

  if ( 0 != result )
    tryNext( TOKEN::SEMICOLON, true );

  // results.push_back({P_TOKEN::DECL, last_offset, offset});
  results.at( __pos__ ).setRange( last_offset, offset );

  if ( 0 == result )
    return 0;

  RETURN_ERROR
}

i32 Preprocess::RetType()
{
  results.push_back( { P_TOKEN::RET, offset, offset + 1 } );
  Next();

  i32 res = Binary( [this] { return Current() == TOKEN::SEMICOLON; } );
  if ( 0 != res ) {
    RETURN_ERROR
  }

  return 0;
}

i32 Preprocess::LoopW()
{
  results.push_back( { P_TOKEN::LOOP, offset, offset + 1 } );
  current_scope->getChildat( current_scope->idchild_ )->setBreakable( true );

  tryNext( TOKEN::NLBRAC, true );

  int res = Binary( [this]() { return Current() == TOKEN::OBRACE; } ); // 暂时不支持不带{}的while
  if ( 0 != res ) {
    RETURN_ERROR
  }

  return 0;
}

int Preprocess::LoopF()
{
  return {};
}

int Preprocess::LoopD()
{
  return {};
}

const p_token_t& Preprocess::getNext()
{
  if ( id >= results.size() ) {
    EXIT_ERROR
  }
  return static_cast<const p_token_t&>( results[id++] );
}

u32 Preprocess::getSize() const
{
  return static_cast<u32>( results.size() );
}

const std::vector<p_token_t>& Preprocess::getResult()
{
  return results;
}

Scope::pointer Preprocess::CurrentScope()
{
  return current_scope;
}

};

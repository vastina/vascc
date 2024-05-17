#include "codegen.hpp"
#include "base/io.hpp"
#include "base/log.hpp"
#include "stmt.hpp"

namespace vastina {

void Generator::Generate( const string_view& file_name )
{
  filer_ = new Filer( file_name );

  filer_->PushBack( x86::file_start( file_name ) );
  EXCEPT_ZERO( doGenerate, current_stmt_ );
  filer_->PushBack( x86::asm_end_str );
}

i32 Generator::doGenerate( Stmt::pointer stmt )
{
  static u32 pos;
  switch ( stmt->StmtType() ) {
    case STMTTYPE::Fdecl: {
      if ( nullptr != stmt->getParent() ) {
        if ( STMTTYPE::Fdecl == stmt->getParent()->StmtType() ) {
          // todo, func declared in func
        }
      }
      pos = filer_->PushBack( x86::func_declare_start( stmt->getFunc()->getName() ) );
      filer_->PushBack( x86::func_start( stmt->getFunc()->getName(), counter_.lf.lfbe ) );
      break;
    }
    case STMTTYPE::Vdecl: {
      auto type { stmt->getVar()->getType() };
      switch ( type ) {
        case TOKEN::INT:

        case TOKEN::LONG:

        default:
          THIS_NOT_SUPPORT( std::format( "type with token id {},", (i32)type ) );
      }
    }
    default:
      break;
  }

  for ( auto&& child : stmt->getChildren() ) {
    doGenerate( child );
  }

  switch ( stmt->StmtType() ) {
    case STMTTYPE::Fdecl: {
      if ( stmt->getFunc()->getSrcloc().token == TOKEN::MAIN )
        filer_->PushBack( x86::main_func_end );
      else
        filer_->PushBack( x86::func_end );
      filer_->PushBack( x86::func_declare_end( counter_.lf.lfbe++, stmt->getFunc()->getName() ) );
    }
    case STMTTYPE::Compound:
    case STMTTYPE::Vdecl:
    default:
      break;
  }

  return 0;
}

}; // namespace vastina

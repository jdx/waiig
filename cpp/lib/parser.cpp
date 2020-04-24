#include "monkey/parser.h"

using std::move;

namespace monkey {
Parser::Parser(Lexer& lexer)
    : lexer{lexer}
    , cur_token{lexer.next_token()}
    , peek_token{lexer.next_token()} { }

void Parser::next_token() {
  cur_token  = move(peek_token);
  peek_token = lexer.next_token();
}
} // namespace monkey

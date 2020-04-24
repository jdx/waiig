#include "monkey/parser.h"

#include "fmt/ostream.h"

using namespace fmt::literals;
using std::make_unique;
using std::move;
using std::unique_ptr;

namespace monkey {
using TT = Token::Type;

Parser::Parser(Lexer& lexer)
    : lexer{lexer}
    , cur_token{lexer.next_token()}
    , peek_token{lexer.next_token()} { }

void Parser::next_token() {
  cur_token  = move(peek_token);
  peek_token = lexer.next_token();
}
Program Parser::parse_program() {
  Program p{};
  while (cur_token.type != Token::Type::EOF_) {
    auto stmt = parse_statement();
    if (stmt) p.statements.push_back(move(stmt));
    next_token();
  }
  return p;
}

unique_ptr<Statement> Parser::parse_statement() {
  switch (cur_token.type) {
  case Token::Type::LET: {
    return parse_let_statement();
  }
  default: {
    // print("invalid token: {}\n", cur_token);
    return nullptr;
  }
  }
}
std::unique_ptr<LetStatement> Parser::parse_let_statement() {
  auto stmt = make_unique<LetStatement>(move(cur_token));
  if (!expect_peek(TT::IDENT)) { return nullptr; }
  stmt->name = make_unique<Identifier>(move(cur_token));
  if (!expect_peek(TT::ASSIGN)) { return nullptr; }
  while (!cur_token_is(TT::SEMICOLON)) { next_token(); }
  return stmt;
}

bool Parser::cur_token_is(Token::Type type) const {
  return cur_token.type == type;
}

bool Parser::peek_token_is(Token::Type type) const {
  return peek_token.type == type;
}

bool Parser::expect_peek(Token::Type type) {
  if (peek_token_is(type)) {
    next_token();
    return true;
  }
  peek_error(type);
  return false;
}
void Parser::peek_error(TT expected) {
  errors.push_back("expected next token to be {} but got {}"_format(
      expected, peek_token.type));
}

} // namespace monkey

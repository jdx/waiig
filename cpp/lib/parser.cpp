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
    , peek_token{lexer.next_token()} {
  prefix_parse_fns[TT::IDENT] = std::bind(&Parser::parse_identifier, this);
}

void Parser::register_prefix(Token::Type type, Parser::PrefixParseFn fn) {
  prefix_parse_fns[type] = fn;
}
void Parser::register_infix(Token::Type type, Parser::InfixParseFn fn) {
  infix_parse_fns[type] = fn;
}

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
  case Token::Type::LET: return parse_let_statement();
  case Token::Type::RETURN: return parse_return_statement();
  default: return parse_expression_statement();
  }
}
unique_ptr<LetStatement> Parser::parse_let_statement() {
  auto stmt = make_unique<LetStatement>(move(cur_token));
  if (!expect_peek(TT::IDENT)) { return nullptr; }
  stmt->name = make_unique<Identifier>(move(cur_token));
  if (!expect_peek(TT::ASSIGN)) { return nullptr; }
  while (!cur_token_is(TT::SEMICOLON)) { next_token(); }
  return stmt;
}

unique_ptr<ReturnStatement> Parser::parse_return_statement() {
  auto stmt = make_unique<ReturnStatement>(move(cur_token));
  while (!cur_token_is(TT::SEMICOLON)) next_token();
  return stmt;
}

unique_ptr<ExpressionStatement> Parser::parse_expression_statement() {
  auto stmt        = make_unique<ExpressionStatement>(cur_token);
  stmt->expression = parse_expression(Precedence::LOWEST);

  if (peek_token_is(TT::SEMICOLON)) { next_token(); }
  return stmt;
}

unique_ptr<Expression> Parser::parse_expression(Precedence precedence) {
  auto fn = prefix_parse_fns.find(cur_token.type);
  if (fn == prefix_parse_fns.end()) { return nullptr; }
  auto left_exp = fn->second();
  return left_exp;
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
Parser::ExpressionPtr Parser::parse_identifier() {
  auto exp = make_unique<Identifier>(move(cur_token));
  return exp;
}

} // namespace monkey

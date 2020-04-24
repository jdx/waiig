#pragma once

#include "ast.h"
#include "lexer.h"
#include "token.h"

namespace monkey {

struct Parser {
  Lexer& lexer;
  Token cur_token;
  Token peek_token;
  std::vector<std::string> errors{};

  explicit Parser(Lexer& lexer);
  Program parse_program();

private:

  void next_token();

  std::unique_ptr<Statement> parse_statement();
  std::unique_ptr<LetStatement> parse_let_statement();

  bool cur_token_is(Token::Type type) const;
  bool peek_token_is(Token::Type type) const;
  bool expect_peek(Token::Type type);
  void peek_error(Token::Type type);
};

}

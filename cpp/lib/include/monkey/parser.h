#pragma once

#include "ast.h"
#include "lexer.h"
#include "token.h"

namespace monkey {

struct Parser {
  Lexer& lexer;
  Token cur_token;
  Token peek_token;

  explicit Parser(Lexer& lexer);

  void next_token();
};

}

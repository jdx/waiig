#include "monkey/ast.h"

namespace monkey {

using std::string;

const std::string& Program::token_literal() {
  static string empty;
  if (statements.empty()) { return empty; }
  return statements[0].token_literal();
}

LetStatement::LetStatement(const Token& token,
                           const Identifier& name,
                           const Expression& value)
    : token{token}
    , name{name}
    , value{value} { }

LetStatement::~LetStatement() = default;
const std::string& LetStatement::token_literal() {
  return token.literal;
}

Identifier::Identifier(const Token& token, const std::string& value)
    : token{token}
    , value{value} { }
Identifier::~Identifier() = default;
const std::string& Identifier::token_literal() {
  return token.literal;
}
} // namespace monkey

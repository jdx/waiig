#include "monkey/ast.h"

namespace monkey {

using std::string;

const std::string& Program::token_literal() const {
  static string empty;
  if (statements.empty()) { return empty; }
  return statements[0]->token_literal();
}

LetStatement::LetStatement(Token&& token)
    : token{std::move(token)} { }

LetStatement::~LetStatement() = default;
const std::string& LetStatement::token_literal() const {
  return token.literal;
}

Identifier::Identifier(Token&& token)
    : token{std::move(token)}
    , value{this->token.literal} { }
Identifier::~Identifier() = default;
const std::string& Identifier::token_literal() const {
  return token.literal;
}
} // namespace monkey

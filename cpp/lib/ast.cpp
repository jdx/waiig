#include "monkey/ast.h"

namespace monkey {

using std::move;
using std::string;

const string& Program::token_literal() const {
  static string empty;
  if (statements.empty()) { return empty; }
  return statements[0]->token_literal();
}

LetStatement::LetStatement(Token&& token)
    : token{move(token)} { }

LetStatement::~LetStatement() = default;
const string& LetStatement::token_literal() const {
  return token.literal;
}

Identifier::Identifier(Token&& token)
    : token{move(token)}
    , value{this->token.literal} { }
Identifier::~Identifier() = default;
const string& Identifier::token_literal() const {
  return token.literal;
}
ReturnStatement::ReturnStatement(Token&& token)
    : token{move(token)} { }
ReturnStatement::~ReturnStatement() = default;
const string& ReturnStatement::token_literal() const {
  return token.literal;
}
} // namespace monkey

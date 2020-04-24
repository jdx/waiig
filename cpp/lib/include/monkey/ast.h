#pragma once

#include <string>
#include <vector>

#include "monkey/token.h"

namespace monkey {

struct Node {
  virtual ~Node() = default;

  virtual const std::string& token_literal() = 0;
};

struct Statement : Node {
  void statementNode() { }
};

struct Expression : Node {
  void expressionNode() { }
};

struct Program {
  std::vector<Statement> statements{};

  const std::string& token_literal();
};

struct Identifier : Expression {
  const Token& token; ///> token.IDENT
  const std::string& value;

  Identifier(const Token& token,
             const std::string& value);
  ~Identifier() override;
  const std::string& token_literal() override;
};

struct LetStatement : Statement {
  const Token& token;
  const Identifier& name;
  const Expression& value;

  LetStatement(const Token& token,
               const Identifier& name,
               const Expression& value);
  ~LetStatement() override;
  const std::string& token_literal() override;
};

} // namespace monkey

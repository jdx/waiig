#pragma once

#include <string>
#include <vector>

#include "monkey/token.h"

namespace monkey {

struct Node {
  virtual ~Node() = default;

  virtual const std::string& token_literal() const = 0;
};

struct Statement : Node {
  // void statementNode() { }
};

struct Expression : Node {
  // void expressionNode() { }
};

struct Program : Node {
  std::vector<std::unique_ptr<Statement>> statements{};

  const std::string& token_literal() const override;
};

struct Identifier : Expression {
  Token token; ///> token.IDENT
  const std::string& value;

  explicit Identifier(Token&& token);
  ~Identifier() override;
  const std::string& token_literal() const override;
};

struct LetStatement : Statement {
  Token token;
  std::unique_ptr<Identifier> name{};
  std::unique_ptr<Expression> value{};

  explicit LetStatement(Token&& token);
  ~LetStatement() override;
  const std::string& token_literal() const override;
};

struct ReturnStatement : Statement {
  Token token;
  std::unique_ptr<Expression> return_value;

  explicit ReturnStatement(Token&& token);
  ~ReturnStatement() override;
  const std::string& token_literal() const override;
};

} // namespace monkey

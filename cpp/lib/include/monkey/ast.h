#pragma once

#include <string>
#include <vector>

#include "monkey/token.h"

namespace monkey {

struct Node {
  Token token;
  explicit Node(Token&& token);
  virtual ~Node() = default;

  virtual const std::string& token_literal() const;

  friend std::ostream& operator<<(std::ostream&, const Node&);
  virtual std::string to_str() const;
};

struct Statement : Node {
  explicit Statement(Token&& token);
  // void statementNode() { }
};

struct Expression : Node {
  explicit Expression(Token&& token);
  // void expressionNode() { }
};

struct Program {
  std::vector<std::unique_ptr<Statement>> statements{};
  const std::string& token_literal() const;
  friend std::ostream& operator<<(std::ostream&, const Program&);
};

struct Identifier : Expression {
  explicit Identifier(Token token);

  const std::string& value;

  std::string to_str() const override;
};

struct LetStatement : Statement {
  std::unique_ptr<Identifier> name{};
  std::unique_ptr<Expression> value{};
  explicit LetStatement(Token token);

  std::string to_str() const override;
};

struct ReturnStatement : Statement {
  std::unique_ptr<Expression> return_value;
  explicit ReturnStatement(Token token);

  std::string to_str() const override;
};

struct ExpressionStatement : Statement {
  std::unique_ptr<Expression> expression;
  explicit ExpressionStatement(Token token);

  std::string to_str() const override;
};

struct IntegerLiteral : Expression {
  int value;
  explicit IntegerLiteral(Token token);
};

struct PrefixExpression : Expression {
  std::string op;
  std::unique_ptr<Expression> right;
  explicit PrefixExpression(Token token);
  std::string to_str() const override;
};

struct InfixExpression : Expression {
  std::string op;
  std::unique_ptr<Expression> left;
  std::unique_ptr<Expression> right;
  explicit InfixExpression(Token token, std::unique_ptr<Expression>&& left);
  std::string to_str() const override;
};

} // namespace monkey

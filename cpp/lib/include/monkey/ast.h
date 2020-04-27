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

protected:
  virtual std::ostream& print(std::ostream&) const;
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

struct BlockStatement : Statement {
  explicit BlockStatement(Token&& token);

  std::vector<std::unique_ptr<Statement>> statements{};

  std::ostream& print(std::ostream&) const override;
};

struct Identifier : Expression {
  explicit Identifier(Token token);

  std::string value;

  std::ostream& print(std::ostream&) const override;
};

struct LetStatement : Statement {
  explicit LetStatement(Token token);

  std::unique_ptr<Identifier> name{};
  std::unique_ptr<Expression> value{};

  std::ostream& print(std::ostream&) const override;
};

struct ReturnStatement : Statement {
  explicit ReturnStatement(Token token);

  std::unique_ptr<Expression> return_value;

  std::ostream& print(std::ostream&) const override;
};

struct ExpressionStatement : Statement {
  explicit ExpressionStatement(Token token);

  std::unique_ptr<Expression> expression;

  std::ostream& print(std::ostream&) const override;
};

struct IntegerLiteral : Expression {
  explicit IntegerLiteral(Token token);

  int value;
};

struct PrefixExpression : Expression {
  explicit PrefixExpression(Token token);

  std::string op;
  std::unique_ptr<Expression> right;

  std::ostream& print(std::ostream&) const override;
};

struct InfixExpression : Expression {
  explicit InfixExpression(Token token, std::unique_ptr<Expression>&& left);

  std::string op;
  std::unique_ptr<Expression> left;
  std::unique_ptr<Expression> right;

  std::ostream& print(std::ostream&) const override;
};

struct Boolean : Expression {
  explicit Boolean(Token&& token);

  bool value;
};

struct IfExpression : Expression {
  explicit IfExpression(Token&& token);

  std::unique_ptr<Expression> condition;
  std::unique_ptr<BlockStatement> consequence;
  std::unique_ptr<BlockStatement> alternative;

  std::ostream& print(std::ostream&) const override;
};

struct FunctionLiteral : Expression {
  explicit FunctionLiteral(Token&& token);

  std::vector<Identifier> parameters{};
  std::unique_ptr<BlockStatement> body;

  std::ostream& print(std::ostream&) const override;
};

} // namespace monkey

#include "monkey/ast.h"

#include <fmt/ostream.h>

#include <sstream>

namespace monkey {

using namespace fmt::literals;
using std::move;
using std::ostream;
using std::string;
using ExpressionPtr = std::unique_ptr<Expression>;

//<editor-fold desc="Node">
Node::Node(Token&& token)
    : token{move(token)} { }

const string& Node::token_literal() const {
  return token.literal;
}

ostream& operator<<(ostream& out, const Node& n) {
  return n.print(out);
}
std::ostream& Node::print(ostream& out) const {
  return out << token_literal();
}
//</editor-fold>

//<editor-fold desc="Program">
const string& Program::token_literal() const {
  static string empty;
  if (statements.empty()) { return empty; }
  return statements[0]->token_literal();
}
ostream& operator<<(ostream& out, const Program& p) {
  for (const auto& s : p.statements) out << *s;
  return out;
}
//</editor-fold>

Statement::Statement(Token&& token)
    : Node(move(token)) { }

BlockStatement::BlockStatement(Token&& token)
    : Statement(move(token)) { }

std::ostream& BlockStatement::print(ostream& out) const {
  out << "{";
  for (const auto& s : statements) out << " " << *s;
  return out << " }";
}

Expression::Expression(Token&& token)
    : Node(move(token)) { }

Identifier::Identifier(Token token)
    : Expression{move(token)}
    , value{this->token.literal} { }

std::ostream& Identifier::print(ostream& out) const {
  return out << value;
}

LetStatement::LetStatement(Token token)
    : Statement{move(token)} { }

std::ostream& LetStatement::print(ostream& out) const {
  out << token_literal() << " " << *name << " = ";
  if (value) out << *value;
  return out << ";";
}

ExpressionStatement::ExpressionStatement(Token token)
    : Statement{move(token)} { }

std::ostream& ExpressionStatement::print(ostream& out) const {
  if (expression) out << *expression;
  return out;
}

ReturnStatement::ReturnStatement(Token token)
    : Statement{move(token)} { }

std::ostream& ReturnStatement::print(ostream& out) const {
  return out << token_literal() << " ";
  if (return_value) out << *return_value;
  return out << ";";
}

IntegerLiteral::IntegerLiteral(Token token)
    : Expression{move(token)} { }

PrefixExpression::PrefixExpression(Token token)
    : Expression{move(token)}
    , op{this->token.literal} { }

std::ostream& PrefixExpression::print(ostream& out) const {
  return out << "(" << op << *right << ")";
}

InfixExpression::InfixExpression(Token token, ExpressionPtr&& left)
    : Expression{move(token)}
    , op{this->token.literal}
    , left{move(left)} { }

std::ostream& InfixExpression::print(ostream& out) const {
  return out << "(" << *left << " " << op << " " << *right << ")";
}

Boolean::Boolean(Token&& token)
    : Expression{move(token)} { }

std::ostream& IfExpression::print(ostream& out) const {
  out << "if " << *condition << " " << *consequence;
  if (alternative) out << " else " << *alternative;
  return out;
}
IfExpression::IfExpression(Token&& token)
    : Expression{move(token)} { }

FunctionLiteral::FunctionLiteral(Token&& token)
    : Expression{move(token)} { }

std::ostream& FunctionLiteral::print(ostream& out) const {
  out << token_literal() << "(";
  bool first{true};
  for (auto& p : parameters) {
    if (!first) out << ", ";
    out << p;
    if (first) first = false;
  }
  return out << ") " << *body;
}

} // namespace monkey

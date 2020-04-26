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
std::string Node::to_str() const {
  return token_literal();
}

ostream& operator<<(ostream& out, const Node& n) {
  return out << n.to_str();
}
//</editor-fold>

//<editor-fold desc="Program">
const string& Program::token_literal() const {
  static string empty;
  if (statements.empty()) { return empty; }
  return statements[0]->token_literal();
}
ostream& operator<<(ostream& out, const Program& p) {
  return out << p.to_str();
}
std::string Program::to_str() const {
  std::stringstream ss{};
  for (const auto& s : statements) ss << *s;
  return ss.str();
}
//</editor-fold>

Statement::Statement(Token&& token)
    : Node(move(token)) { }

BlockStatement::BlockStatement(Token&& token)
    : Statement(move(token)) { }

std::string BlockStatement::to_str() const {
  std::stringstream ss{};
  ss << "{";
  for (const auto& s : statements) ss << " " << *s;
  ss << " }";
  return ss.str();
}

Expression::Expression(Token&& token)
    : Node(move(token)) { }

Identifier::Identifier(Token token)
    : Expression{move(token)}
    , value{this->token.literal} { }
std::string Identifier::to_str() const {
  return value;
}

LetStatement::LetStatement(Token token)
    : Statement{move(token)} { }
std::string LetStatement::to_str() const {
  return "{} {} = {};"_format(
      token_literal(), *name, value ? "{}"_format(*value) : "");
}

ExpressionStatement::ExpressionStatement(Token token)
    : Statement{move(token)} { }

std::string ExpressionStatement::to_str() const {
  if (!expression) return "";
  return "{}"_format(*expression);
}

ReturnStatement::ReturnStatement(Token token)
    : Statement{move(token)} { }

std::string ReturnStatement::to_str() const {
  return "{} {};"_format(token_literal(),
                         return_value ? "{}"_format(*return_value) : "");
}

IntegerLiteral::IntegerLiteral(Token token)
    : Expression{move(token)} { }

PrefixExpression::PrefixExpression(Token token)
    : Expression{move(token)}
    , op{this->token.literal} { }

std::string PrefixExpression::to_str() const {
  return "({}{})"_format(op, *right);
}

InfixExpression::InfixExpression(Token token, ExpressionPtr&& left)
    : Expression{move(token)}
    , op{this->token.literal}
    , left{move(left)} { }

std::string InfixExpression::to_str() const {
  return "({} {} {})"_format(*left, op, *right);
}

Boolean::Boolean(Token&& token)
    : Expression{move(token)} { }

std::string IfExpression::to_str() const {
  if (alternative) {
    return "if {} {} else {}"_format(*condition, *consequence, *alternative);
  } else {
    return "if {} {}"_format(*condition, *consequence);
  }
}
IfExpression::IfExpression(Token&& token)
    : Expression{move(token)} { }
} // namespace monkey

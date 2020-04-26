#pragma once

#include <unordered_map>

#include "ast.h"
#include "lexer.h"
#include "token.h"

namespace monkey {

struct Parser {
  Lexer& lexer;
  Token cur_token;
  Token peek_token;
  std::vector<std::string> errors{};

  explicit Parser(Lexer& lexer);
  Program parse_program();

private:
  using ExpressionPtr = std::unique_ptr<Expression>;
  using PrefixParseFn = std::function<ExpressionPtr()>;
  using InfixParseFn  = std::function<ExpressionPtr(ExpressionPtr&&)>;
  enum class Precedence {
    LOWEST,
    EQUALS,      ///< ==
    LESSGREATER, ///< > or <
    SUM,         ///< +
    PRODUCT,     ///< *
    PREFIX,      ///< -X or !X
    CALL,        ///< myFunction(X)
  };

  std::unordered_map<Token::Type, PrefixParseFn> prefix_parse_fns{};
  std::unordered_map<Token::Type, InfixParseFn> infix_parse_fns{};

  std::unique_ptr<Statement> parse_statement();
  std::unique_ptr<LetStatement> parse_let_statement();
  std::unique_ptr<ReturnStatement> parse_return_statement();
  std::unique_ptr<ExpressionStatement> parse_expression_statement();
  std::unique_ptr<Expression> parse_expression(Precedence precedence);
  std::unique_ptr<IntegerLiteral> parse_integer_literal();
  ExpressionPtr parse_identifier();
  ExpressionPtr parse_prefix_expression();
  ExpressionPtr parse_infix_expression(ExpressionPtr&& left);
  ExpressionPtr parse_boolean();
  ExpressionPtr parse_grouped_expression();

  void next_token();
  bool cur_token_is(Token::Type type) const;
  bool peek_token_is(Token::Type type) const;
  bool expect_peek(Token::Type type);
  void peek_error(Token::Type type);
  void no_prefix_parse_fn_error(Token::Type type);

  static std::unordered_map<Token::Type, Precedence> precedences;

  Precedence peek_precedence();
  Precedence cur_precedence();
};

} // namespace monkey

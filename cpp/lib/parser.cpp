#include "monkey/parser.h"

#include "fmt/ostream.h"

using namespace fmt::literals;
using std::make_unique;
using std::move;
using std::unique_ptr;
using std::vector;

namespace monkey {
using TT            = Token::Type;
using ExpressionPtr = unique_ptr<Expression>;

Parser::Parser(Lexer& lexer)
    : lexer{lexer}
    , cur_token{lexer.next_token()}
    , peek_token{lexer.next_token()} {
  using namespace std::placeholders;

  prefix_parse_fns[TT::IDENT] = std::bind(&Parser::parse_identifier, this);
  prefix_parse_fns[TT::INT]   = std::bind(&Parser::parse_integer_literal, this);
  prefix_parse_fns[TT::BANG] =
      std::bind(&Parser::parse_prefix_expression, this);
  prefix_parse_fns[TT::MINUS] =
      std::bind(&Parser::parse_prefix_expression, this);
  prefix_parse_fns[TT::TRUE]  = std::bind(&Parser::parse_boolean, this);
  prefix_parse_fns[TT::FALSE] = std::bind(&Parser::parse_boolean, this);
  prefix_parse_fns[TT::LPAREN] =
      std::bind(&Parser::parse_grouped_expression, this);
  prefix_parse_fns[TT::IF] = std::bind(&Parser::parse_if_expression, this);
  prefix_parse_fns[TT::FUNCTION] =
      std::bind(&Parser::parse_function_literal, this);

  infix_parse_fns[TT::PLUS] =
      std::bind(&Parser::parse_infix_expression, this, _1);
  infix_parse_fns[TT::MINUS] =
      std::bind(&Parser::parse_infix_expression, this, _1);
  infix_parse_fns[TT::SLASH] =
      std::bind(&Parser::parse_infix_expression, this, _1);
  infix_parse_fns[TT::ASTERISK] =
      std::bind(&Parser::parse_infix_expression, this, _1);
  infix_parse_fns[TT::EQ] =
      std::bind(&Parser::parse_infix_expression, this, _1);
  infix_parse_fns[TT::NOT_EQ] =
      std::bind(&Parser::parse_infix_expression, this, _1);
  infix_parse_fns[TT::LT] =
      std::bind(&Parser::parse_infix_expression, this, _1);
  infix_parse_fns[TT::GT] =
      std::bind(&Parser::parse_infix_expression, this, _1);
  infix_parse_fns[TT::LPAREN] =
      std::bind(&Parser::parse_call_expression, this, _1);
}

void Parser::next_token() {
  cur_token  = std::move(peek_token);
  peek_token = lexer.next_token();
}

Program Parser::parse_program() {
  Program p{};
  while (cur_token.type != Token::Type::EOF_) {
    auto stmt = parse_statement();
    if (stmt) p.statements.push_back(std::move(stmt));
    next_token();
  }
  return p;
}

unique_ptr<Statement> Parser::parse_statement() {
  switch (cur_token.type) {
  case Token::Type::LET: return parse_let_statement();
  case Token::Type::RETURN: return parse_return_statement();
  default: return parse_expression_statement();
  }
}

unique_ptr<LetStatement> Parser::parse_let_statement() {
  auto stmt = make_unique<LetStatement>(cur_token);
  if (!expect_peek(TT::IDENT)) { return nullptr; }
  stmt->name = make_unique<Identifier>(cur_token);
  if (!expect_peek(TT::ASSIGN)) { return nullptr; }
  next_token();
  stmt->value = parse_expression(Precedence::LOWEST);
  if (peek_token_is(TT::SEMICOLON)) next_token();
  return stmt;
}

unique_ptr<ReturnStatement> Parser::parse_return_statement() {
  auto stmt = make_unique<ReturnStatement>(cur_token);
  next_token();
  stmt->return_value = parse_expression(Precedence::LOWEST);
  if (peek_token_is(TT::SEMICOLON)) next_token();
  return stmt;
}

unique_ptr<ExpressionStatement> Parser::parse_expression_statement() {
  auto stmt        = make_unique<ExpressionStatement>(cur_token);
  stmt->expression = parse_expression(Precedence::LOWEST);

  if (peek_token_is(TT::SEMICOLON)) { next_token(); }
  return stmt;
}

unique_ptr<Expression> Parser::parse_expression(Precedence precedence) {
  auto fn = prefix_parse_fns.find(cur_token.type);
  if (fn == prefix_parse_fns.end()) {
    no_prefix_parse_fn_error(cur_token.type);
    return nullptr;
  }
  auto left_exp = fn->second();

  while (!peek_token_is(Token::Type::SEMICOLON)
         && precedence < peek_precedence()) {
    auto s = infix_parse_fns.find(peek_token.type);
    if (s == infix_parse_fns.end()) break;
    next_token();
    left_exp = s->second(move(left_exp));
  }

  return left_exp;
}

bool Parser::cur_token_is(Token::Type type) const {
  return cur_token.type == type;
}

bool Parser::peek_token_is(Token::Type type) const {
  return peek_token.type == type;
}

bool Parser::expect_peek(Token::Type type) {
  if (peek_token_is(type)) {
    next_token();
    return true;
  }
  peek_error(type);
  return false;
}

void Parser::peek_error(TT expected) {
  errors.push_back("expected next token to be {} but got {}"_format(
      expected, peek_token.type));
}

Parser::ExpressionPtr Parser::parse_identifier() {
  auto exp = make_unique<Identifier>(cur_token);
  return exp;
}

unique_ptr<IntegerLiteral> Parser::parse_integer_literal() {
  auto exp   = make_unique<IntegerLiteral>(cur_token);
  exp->value = std::stoi(cur_token.literal);
  return exp;
}

void Parser::no_prefix_parse_fn_error(Token::Type type) {
  errors.emplace_back("no prefix parse function for {} found"_format(type));
}

Parser::ExpressionPtr Parser::parse_prefix_expression() {
  auto exp = make_unique<PrefixExpression>(cur_token);
  next_token();
  exp->right = parse_expression(Precedence::PREFIX);
  return exp;
}

std::unordered_map<TT, Parser::Precedence> Parser::precedences = {
    {TT::EQ, Precedence::EQUALS},
    {TT::NOT_EQ, Precedence::EQUALS},
    {TT::LT, Precedence::LESSGREATER},
    {TT::GT, Precedence::LESSGREATER},
    {TT::PLUS, Precedence::SUM},
    {TT::MINUS, Precedence::SUM},
    {TT::SLASH, Precedence::PRODUCT},
    {TT::ASTERISK, Precedence::PRODUCT},
    {TT::LPAREN, Precedence::CALL},
};

Parser::Precedence Parser::peek_precedence() {
  auto p = precedences.find(peek_token.type);
  if (p == precedences.end()) return Precedence::LOWEST;
  return p->second;
}

Parser::Precedence Parser::cur_precedence() {
  auto p = precedences.find(cur_token.type);
  if (p == precedences.end()) return Precedence::LOWEST;
  return p->second;
}

Parser::ExpressionPtr Parser::parse_infix_expression(ExpressionPtr&& left) {
  auto exp        = make_unique<InfixExpression>(cur_token, move(left));
  auto precedence = cur_precedence();
  next_token();
  exp->right = parse_expression(precedence);
  return exp;
}

Parser::ExpressionPtr Parser::parse_boolean() {
  auto exp   = make_unique<Boolean>(std::move(cur_token));
  exp->value = cur_token_is(TT::TRUE);
  return exp;
}

Parser::ExpressionPtr Parser::parse_grouped_expression() {
  next_token();
  auto exp = parse_expression(Precedence::LOWEST);

  if (!expect_peek(TT::RPAREN)) return nullptr;

  return exp;
}

Parser::ExpressionPtr Parser::parse_if_expression() {
  auto exp = make_unique<IfExpression>(std::move(cur_token));
  if (!expect_peek(Token::Type::LPAREN)) return nullptr;
  next_token();
  exp->condition = parse_expression(Precedence::LOWEST);
  if (!expect_peek(Token::Type::RPAREN)) { return nullptr; }
  if (!expect_peek(Token::Type::LBRACE)) { return nullptr; }
  exp->consequence = parse_block_statement();

  if (peek_token_is(Token::Type::ELSE)) {
    next_token();
    if (!expect_peek(Token::Type::LBRACE)) return nullptr;
    exp->alternative = parse_block_statement();
  }

  return exp;
}

unique_ptr<BlockStatement> Parser::parse_block_statement() {
  auto block = make_unique<BlockStatement>(move(cur_token));
  next_token();
  while (!cur_token_is(Token::Type::RBRACE)
         && !cur_token_is(Token::Type::EOF_)) {
    auto stmt = parse_statement();
    if (stmt) { block->statements.push_back(move(stmt)); }
    next_token();
  }
  return block;
}

Parser::ExpressionPtr Parser::parse_function_literal() {
  auto exp = make_unique<FunctionLiteral>(move(cur_token));
  if (!expect_peek(Token::Type::LPAREN)) return nullptr;
  exp->parameters = parse_function_parameters();
  if (!expect_peek(Token::Type::LBRACE)) return nullptr;
  exp->body = parse_block_statement();
  return exp;
}

std::vector<Identifier> Parser::parse_function_parameters() {
  std::vector<Identifier> params{};
  if (peek_token_is(Token::Type::RPAREN)) {
    next_token();
    return params;
  }
  next_token();
  params.emplace_back(move(cur_token));

  while (peek_token_is(Token::Type::COMMA)) {
    next_token();
    next_token();
    params.emplace_back(move(cur_token));
  }

  if (!expect_peek(Token::Type::RPAREN)) return params;

  return params;
}
Parser::ExpressionPtr Parser::parse_call_expression(ExpressionPtr func) {
  auto exp       = make_unique<CallExpression>(move(cur_token), move(func));
  exp->arguments = parse_call_arguments();
  return exp;
}

vector<ExpressionPtr> Parser::parse_call_arguments() {
  vector<ExpressionPtr> args{};
  if (peek_token_is(Token::Type::RPAREN)) {
    next_token();
    return args;
  }

  next_token();
  args.emplace_back(parse_expression(Precedence::LOWEST));

  while (peek_token_is(Token::Type::COMMA)) {
    next_token();
    next_token();
    args.emplace_back(parse_expression(Precedence::LOWEST));
  }

  if (!expect_peek(Token::Type::RPAREN)) return args;

  return args;
}

} // namespace monkey

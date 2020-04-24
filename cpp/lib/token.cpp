#include "monkey/token.h"

#include <fmt/ostream.h>

#include <unordered_map>
#include <utility>

using fmt::format;
using std::string;
using std::unordered_map;

namespace monkey {

string type_to_str(Token::Type type) {
  static unordered_map<Token::Type, string> lookup{
      {Token::Type::ILLEGAL, "ILLEGAL"},
      {Token::Type::EOF_, "EOF"},
      {Token::Type::IDENT, "IDENT"},
      {Token::Type::INT, "INT"},
      {Token::Type::ASSIGN, "ASSIGN"},
      {Token::Type::PLUS, "PLUS"},
      {Token::Type::MINUS, "MINUS"},
      {Token::Type::BANG, "BANG"},
      {Token::Type::ASTERISK, "ASTERISK"},
      {Token::Type::SLASH, "SLASH"},
      {Token::Type::GT, "GT"},
      {Token::Type::LT, "LT"},
      {Token::Type::EQ, "EQ"},
      {Token::Type::NOT_EQ, "NOT_EQ"},
      {Token::Type::COMMA, "COMMA"},
      {Token::Type::SEMICOLON, "SEMICOLON"},
      {Token::Type::LPAREN, "LPAREN"},
      {Token::Type::RPAREN, "RPAREN"},
      {Token::Type::LBRACE, "LBRACE"},
      {Token::Type::RBRACE, "RBRACE"},
      {Token::Type::FUNCTION, "FUNCTION"},
      {Token::Type::LET, "LET"},
      {Token::Type::TRUE, "TRUE"},
      {Token::Type::FALSE, "FALSE"},
      {Token::Type::IF, "IF"},
      {Token::Type::ELSE, "ELSE"},
      {Token::Type::RETURN, "RETURN"},
  };
  return lookup.find(type)->second;
}

Token::Type Token::lookup_ident(const std::string& ident) {
  static unordered_map<string, Token::Type> keywords{
      {"fn", Token::Type::FUNCTION},
      {"let", Token::Type::LET},
      {"true", Token::Type::TRUE},
      {"false", Token::Type::FALSE},
      {"if", Token::Type::IF},
      {"else", Token::Type::ELSE},
      {"return", Token::Type::RETURN},
  };

  auto s = keywords.find(ident);
  if (s == keywords.end()) return Type::IDENT;
  return s->second;
}

bool Token::operator==(const Token& other) const {
  return type == other.type && literal == other.literal;
}

ostream& operator<<(ostream& os, const Token& tok) {
  return os << format("{}<\"{}\">", tok.type, tok.literal);
}

ostream& operator<<(ostream& os, Token::Type type) {
  return os << type_to_str(type);
}
Token::Token() = default;
Token::Token(Token::Type type, string literal)
    : type{type}
    , literal{move(literal)} { }
Token::Token(const Token&) = default;
Token::Token(Token&&) noexcept = default;
Token& Token::operator=(Token&&) noexcept = default;
} // namespace monkey

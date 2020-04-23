#include "monkey/token.h"

#include <fmt/ostream.h>

#include <unordered_map>

using fmt::format;
using std::string;
using std::unordered_map;

namespace monkey {

const unordered_map<string, Token::Type> keywords{
    {"fn", Token::Type::FUNCTION},
    {"let", Token::Type::LET},
};

const unordered_map<Token::Type, string> type_lookup{
    {Token::Type::ILLEGAL, "ILLEGAL"},
    {Token::Type::EOF_, "EOF"},
    {Token::Type::IDENT, "IDENT"},
    {Token::Type::INT, "INT"},
    {Token::Type::ASSIGN, "ASSIGN"},
    {Token::Type::PLUS, "PLUS"},
    {Token::Type::COMMA, "COMMA"},
    {Token::Type::SEMICOLON, "SEMICOLON"},
    {Token::Type::LPAREN, "LPAREN"},
    {Token::Type::RPAREN, "RPAREN"},
    {Token::Type::LBRACE, "LBRACE"},
    {Token::Type::RBRACE, "RBRACE"},
    {Token::Type::FUNCTION, "FUNCTION"},
    {Token::Type::LET, "LET"},
};

Token::Type Token::lookup_ident(std::string ident) {
  auto s = keywords.find(ident);
  if (s == keywords.end()) return Type::IDENT;
  return s->second;
}

ostream& operator<<(ostream& os, const Token& tok) {
  return os << format("Token<{}: \"{}\">", tok.type, tok.literal);
}
ostream& operator<<(ostream& os, Token::Type type) {
  return os << type_lookup.find(type)->second;
}

} // namespace monkey
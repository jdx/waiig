#pragma once

#include <string>

using std::ostream;

namespace monkey {
struct Token {
  enum class Type {
    ILLEGAL, // = "ILLEGAL"
    EOF_,    // = "EOF"

    // Identifiers + literals
    IDENT, // = "IDENT" // add, foobar, x, y, ...
    INT,   // = "INT"   // 123456

    // Operators
    ASSIGN, // = "="
    PLUS,   // = "+"

    // Delimiters
    COMMA,     // = ","
    SEMICOLON, // = ";"

    LPAREN, // = "("
    RPAREN, // = ")"
    LBRACE, // = "{"
    RBRACE, // = "}"

    // Keywords
    FUNCTION, // = "FUNCTION"
    LET,      // = "LET"
  };
  friend ostream& operator<<(ostream& os, Type t);

  Type type;
  std::string literal;

  static Type lookup_ident(std::string ident);

  friend ostream& operator<<(ostream& os, const Token& t);
};
} // namespace monkey
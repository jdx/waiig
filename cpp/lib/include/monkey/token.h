#pragma once

#include <string>

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

  Type type;
  std::string literal;
};
} // namespace monkey
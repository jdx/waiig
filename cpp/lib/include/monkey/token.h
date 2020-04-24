#pragma once

#include <string>

using std::ostream;

namespace monkey {
struct Token {
  enum class Type {
    ILLEGAL, ///< Example: "ILLEGAL"
    EOF_,    ///< Example: "EOF"

    // Identifiers + literals
    IDENT, ///< Examples: add, foobar, x, y, ...
    INT,   ///< Example: 123456

    // Operators
    ASSIGN,   ///< Example: "="
    PLUS,     ///< Example: "+"
    MINUS,    ///< Example: "-"
    BANG,     ///< Example: "!"
    ASTERISK, ///< Example: "*"
    SLASH,    ///< Example: "/"

    LT,     ///< Example: "<"
    GT,     ///< Example: ">"
    EQ,     ///< Example: "=="
    NOT_EQ, ///< Example: "!="

    // Delimiters
    COMMA,     ///< Example: ","
    SEMICOLON, ///< Example: ";"

    LPAREN, ///< Example: ")"
    RPAREN, ///< Example: ")"
    LBRACE, ///< Example: "{"
    RBRACE, ///< Example: "}"

    // Keywords
    FUNCTION, ///< Example: "fn"
    LET,      ///< Example: "let"
    TRUE,     ///< Example: "true"
    FALSE,    ///< Example: "false"
    IF,       ///< Example: "if"
    ELSE,     ///< Example: "else"
    RETURN,   ///< Example: "return"
  };
  friend ostream& operator<<(ostream& os, Type t);

  Type type;
  std::string literal{};

  static Type lookup_ident(const std::string& ident);

  bool operator==(const Token&) const;

  friend ostream& operator<<(ostream& os, const Token& t);
};
} // namespace monkey

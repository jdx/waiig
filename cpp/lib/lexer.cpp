#include "monkey/lexer.h"

#include <string>

using std::istream;
using std::make_unique;
using std::string;

namespace monkey {

bool is_whitespace(const char& ch) {
  return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r';
}

bool is_letter(const char& ch) {
  return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || ch == '_';
}

bool is_digit(const char& ch) {
  return '0' <= ch && ch <= '9';
}

Lexer::Lexer(istream& in)
    : in{in} { }
Lexer::Lexer(const string& in)
    : in_str{in} { }
Lexer::~Lexer()                = default;
Lexer::Lexer(Lexer&&) noexcept = default;

Token Lexer::next_token() {
  skip_whitespace();

  Token token{};
  char c;
  if (in >> c) {
    token.literal = {c};
  } else {
    token.literal = {};
    token.type    = Token::Type::EOF_;
    return token;
  }

  switch (c) {
    //<editor-fold desc="single-char">
  case '+': {
    token.type = Token::Type::PLUS;
    break;
  }
  case '-': {
    token.type = Token::Type::MINUS;
    break;
  }
  case '/': {
    token.type = Token::Type::SLASH;
    break;
  }
  case '*': {
    token.type = Token::Type::ASTERISK;
    break;
  }
  case '<': {
    token.type = Token::Type::LT;
    break;
  }
  case '>': {
    token.type = Token::Type::GT;
    break;
  }
  case ';': {
    token.type = Token::Type::SEMICOLON;
    break;
  };
  case ',': {
    token.type = Token::Type::COMMA;
    break;
  }
  case '(': {
    token.type = Token::Type::LPAREN;
    break;
  }
  case ')': {
    token.type = Token::Type::RPAREN;
    break;
  }
  case '{': {
    token.type = Token::Type::LBRACE;
    break;
  }
  case '}': {
    token.type = Token::Type::RBRACE;
    break;
  }
    //</editor-fold>
    //<editor-fold desc="equality">
  case '=': {
    if (in.peek() == '=') {
      in >> c;
      token.literal += c;
      token.type = Token::Type::EQ;
    } else {
      token.type = Token::Type::ASSIGN;
    }
    break;
  };
  case '!': {
    if (in.peek() == '=') {
      in >> c;
      token.literal += c;
      token.type = Token::Type::NOT_EQ;
    } else {
      token.type = Token::Type::BANG;
    }
    break;
  }
    //</editor-fold>
  default: {
    if (is_letter(c)) {
      while (is_letter(in.peek())) {
        in >> c;
        token.literal += c;
      }
      token.type = Token::lookup_ident(token.literal);
    } else if (is_digit(c)) {
      while (is_digit(in.peek())) {
        in >> c;
        token.literal += c;
      }
      token.type = Token::Type::INT;
    } else {
      token.type = Token::Type::ILLEGAL;
    }
  }
  }

  return token;
}

void Lexer::skip_whitespace() {
  while (is_whitespace(in.peek())) in.get();
}

} // namespace monkey

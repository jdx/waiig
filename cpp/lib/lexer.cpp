#include "monkey/lexer.h"

#include <string>

using std::make_unique;
using std::string;

namespace monkey {

struct Lexer::Impl {
  explicit Impl(const string& input)
      : input{input} {
    read_char();
  }

  Token next_token() {
    Token tok{};

    switch (ch) {
    case '=': {
      tok = Token{Token::Type::ASSIGN, {ch}};
      break;
    };
    case ';': {
      tok = Token{Token::Type::SEMICOLON, {ch}};
      break;
    };
    case '(': {
      tok = Token{Token::Type::LPAREN, {ch}};
      break;
    }
    case ')': {
      tok = Token{Token::Type::RPAREN, {ch}};
      break;
    }
    case ',': {
      tok = Token{Token::Type::COMMA, {ch}};
      break;
    }
    case '+': {
      tok = Token{Token::Type::PLUS, {ch}};
      break;
    }
    case '{': {
      tok = Token{Token::Type::LBRACE, {ch}};
      break;
    }
    case '}': {
      tok = Token{Token::Type::RBRACE, {ch}};
      break;
    }
    case 0: {
      tok.literal = "";
      tok.type    = Token::Type::EOF_;
      break;
    }
    }

    read_char();
    return tok;
  }

private:
  const string& input;
  int position{0}; // current reading position in input (points to current char)
  int read_position{0}; // current reading position in input (after ch)
  char ch{0};           // current char under examination

  void read_char() {
    if (read_position >= input.length()) {
      ch = 0;
    } else {
      ch = input[read_position];
    }
    position = read_position;
    ++read_position;
  }
};

Lexer::Lexer(const std::string& input)
    : impl{make_unique<Impl>(input)} { }

Lexer::~Lexer()       = default;
Lexer::Lexer(Lexer&&)  noexcept = default;
Lexer& Lexer::operator=(Lexer&&)  noexcept = default;

Token Lexer::next_token() {
  return impl->next_token();
}
} // namespace monkey

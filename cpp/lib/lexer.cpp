#include "monkey/lexer.h"

#include <string>

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

struct Lexer::Impl {
  explicit Impl(const string& input)
      : input{input} {
    read_char();
  }

  Token next_token() {
    Token tok{};

    skip_whitespace();

    switch (ch) {
      //<editor-fold desc="single-char">
    case '+': {
      tok = {Token::Type::PLUS, {ch}};
      break;
    }
    case '-': {
      tok = {Token::Type::MINUS, {ch}};
      break;
    }
    case '/': {
      tok = {Token::Type::SLASH, {ch}};
      break;
    }
    case '*': {
      tok = {Token::Type::ASTERISK, {ch}};
      break;
    }
    case '<': {
      tok = {Token::Type::LT, {ch}};
      break;
    }
    case '>': {
      tok = {Token::Type::GT, {ch}};
      break;
    }
    case ';': {
      tok = {Token::Type::SEMICOLON, {ch}};
      break;
    };
    case ',': {
      tok = {Token::Type::COMMA, {ch}};
      break;
    }
    case '(': {
      tok = {Token::Type::LPAREN, {ch}};
      break;
    }
    case ')': {
      tok = {Token::Type::RPAREN, {ch}};
      break;
    }
    case '{': {
      tok = {Token::Type::LBRACE, {ch}};
      break;
    }
    case '}': {
      tok = {Token::Type::RBRACE, {ch}};
      break;
    }
      //</editor-fold>
      //<editor-fold desc="equality">
    case '=': {
      if (peek_char() == '=') {
        string a{ch};
        read_char();
        tok = {Token::Type::EQ, a + ch};
      } else {
        tok = {Token::Type::ASSIGN, {ch}};
      }
      break;
    };
    case '!': {
      if (peek_char() == '=') {
        string a{ch};
        read_char();
        tok = {Token::Type::NOT_EQ, a + ch};
      } else {
        tok = {Token::Type::BANG, {ch}};
      }
      break;
    }
      //</editor-fold>
    case 0: {
      tok = {Token::Type::EOF_, ""};
      break;
    }
    default: {
      if (is_letter(ch)) {
        string ident = read_identifier();
        tok          = {Token::lookup_ident(ident), ident};
        return tok;
      } else if (is_digit(ch)) {
        tok = {Token::Type::INT, read_number()};
        return tok;
      } else {
        tok = {Token::Type::ILLEGAL, {ch}};
        return tok;
      }
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

  void skip_whitespace() {
    while (is_whitespace(ch)) read_char();
  }

  char peek_char() {
    if (read_position >= input.length())
      return 0;
    else
      return input[read_position];
  }

  string read_identifier() {
    int start = position;
    while (is_letter(ch)) read_char();
    return input.substr(start, position - start);
  }

  string read_number() {
    int start = position;
    while (is_digit(ch)) read_char();
    return input.substr(start, position - start);
  }
};

Lexer::Lexer(const std::string& input)
    : impl{make_unique<Impl>(input)} { }

Lexer::~Lexer()                = default;
Lexer::Lexer(Lexer&&) noexcept = default;
Lexer& Lexer::operator=(Lexer&&) noexcept = default;

Token Lexer::next_token() {
  return impl->next_token();
}
} // namespace monkey

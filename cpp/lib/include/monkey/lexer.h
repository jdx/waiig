#pragma once

#include <optional>
#include <sstream>
#include <string>

#include "token.h"

namespace monkey {

struct Lexer {
  Lexer(std::istream& in);
  Lexer(const std::string& in);
  ~Lexer();
  Lexer(Lexer&&) noexcept;

  struct Iter {
    explicit Iter(std::istream& in);
    ~Iter();
    Iter(Iter&&) noexcept;

    Token operator*() const;
    Iter& operator++();
    bool operator==(const Iter& other) const;

  private:
    std::istream& in;
    Token token;

    void skip_whitespace();
  };

  Iter begin() const;
  Iter end() const;

private:
  std::istringstream in_str;
  std::istream& in{in_str};
};

Lexer lex(std::istream& in);
Lexer lex(const std::string& in);

} // namespace monkey

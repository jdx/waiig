#pragma once

#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "token.h"

namespace monkey {

struct Lexer {
  explicit Lexer(std::istream& in);
  explicit Lexer(const std::string& in);
  ~Lexer();
  Lexer(Lexer&&) noexcept;

  Token next_token();

private:
  std::istringstream in_str;
  std::istream& in{in_str};
  std::vector<char> literal{};

  void skip_whitespace();
};

} // namespace monkey

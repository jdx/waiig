#pragma once

#include <string>

#include "token.h"

namespace monkey {
struct Lexer {
  explicit Lexer(const std::string& input);
  ~Lexer();
  Lexer(Lexer&&) noexcept;
  Lexer& operator=(Lexer&&) noexcept;

  Token next_token();

private:
  struct Impl;
  std::unique_ptr<Impl> impl;
};
} // namespace monkey

#pragma once

#include "lexer.h"

namespace monkey {
struct Repl {
  static void start(std::istream& in, std::ostream& out);
};
} // namespace monkey
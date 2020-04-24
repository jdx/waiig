#include "monkey/repl.h"
#include "monkey/lexer.h"

#include <fmt/ostream.h>

#include <iostream>

namespace monkey::repl {

using std::istream;
using std::ostream;
using std::runtime_error;
using std::string;
using namespace fmt::literals;

const auto PROMPT = ">> ";

void start(istream& in, ostream& out) {
  out << PROMPT;
  string line;
  while (std::getline(in, line)) {
    for (const auto& tok : lex(line)) {
      out << fmt::format("{}\n", tok);
    }
    out << PROMPT;
  }
  if (in.bad()) { throw runtime_error{"error reading input"}; }
}

} // namespace monkey::repl

#include "monkey/repl.h"

#include <fmt/ostream.h>

#include <iostream>

#include "monkey/lexer.h"

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
    Lexer lex{line};
    for (auto&& tok = lex.next_token(); tok.type != Token::Type::EOF_;
         tok      = lex.next_token()) {
      out << fmt::format("{}\n", tok);
    }
    out << PROMPT;
  }
  if (in.bad()) { throw runtime_error{"error reading input"}; }
  out << "\n";
}

} // namespace monkey::repl

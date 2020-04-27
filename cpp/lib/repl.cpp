#include "monkey/repl.h"

#include <fmt/ostream.h>
#include <fmt/color.h>
#include <monkey/parser.h>

#include <iostream>

#include "monkey/lexer.h"

namespace monkey::repl {

using std::istream;
using std::ostream;
using std::runtime_error;
using std::string;
using std::endl;
using namespace fmt::literals;

const auto PROMPT = ">> ";

void print_parser_errors(ostream& out, const std::vector<string> &errors) {
  for (auto& err : errors) {
    out << fmt::format(fmt::fg(fmt::terminal_color::red), "* {}", err) << endl;
  }
}

void start(istream& in, ostream& out) {
  out << PROMPT;
  string line;
  while (std::getline(in, line)) {
    Lexer lex{line};
    Parser parser{lex};
    Program program = parser.parse_program();
    if (!parser.errors.empty()) {
      print_parser_errors(out, parser.errors);
      out << PROMPT;
      continue;
    }
    out << program << endl << PROMPT;
  }
  if (in.bad()) { throw runtime_error{"error reading input"}; }
  out << "\n";
}

} // namespace monkey::repl

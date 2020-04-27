#include "monkey/evaluator.h"

#include <fmt/ostream.h>
#include <monkey/lexer.h>
#include <monkey/parser.h>

#include <catch2/catch.hpp>
#include <iostream>

using namespace fmt::literals;
using namespace monkey;
using std::string;
using std::unique_ptr;

unique_ptr<object::Object> test_eval_node(string input) {
  Lexer lex{input};
  Parser parse{lex};
  Program program = parse.parse_program();
  return eval(program);
}

string test_eval(string input) {
  return test_eval_node(input)->Inspect();
}

TEST_CASE("evaluator") {
  SECTION("integer") {
    auto e = test_eval_node("5");
    auto i = dynamic_cast<object::Integer&>(*e);
    REQUIRE(i.value == 5);
  };
}

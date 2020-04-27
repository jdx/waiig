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
  return test_eval_node(input)->inspect();
}

TEST_CASE("evaluator") {
  SECTION("integer") {
    SECTION("5") {
      auto e = test_eval_node("5");
      auto i = dynamic_cast<object::Integer&>(*e);
      REQUIRE(i.value == 5);
    };
    SECTION("10") {
      auto e = test_eval_node("10");
      auto i = dynamic_cast<object::Integer&>(*e);
      REQUIRE(i.value == 10);
    };
  };
  SECTION("bool") {
    SECTION("true"){
      auto e = test_eval_node("true");
      auto i = dynamic_cast<object::Boolean&>(*e);
      REQUIRE(i.value);
    };
    SECTION("false"){
      auto e = test_eval_node("false");
      auto i = dynamic_cast<object::Boolean&>(*e);
      REQUIRE_FALSE(i.value);
    };
  };
  SECTION("bang") {
    struct Test {
      string input;
      bool expected;
    };
    std::vector<Test> tests {
        {"!true", false},
        {"!false", true},
        {"!5", false},
        {"!!true", true},
        {"!!false", false},
        {"!!5", true},
    };
    for (auto&& tt : tests) {
      auto e = test_eval_node(tt.input);
      auto i = dynamic_cast<object::Boolean&>(*e);
      REQUIRE(i.value == tt.expected);
    }
  };
  SECTION("negative") {
    struct Test {
      string input;
      int expected;
    };
    std::vector<Test> tests {
        {"5", 5},
        {"10", 10},
        {"-5", -5},
        {"-10", -10},
    };
    for (auto&& tt : tests) {
      auto e = test_eval_node(tt.input);
      auto i = dynamic_cast<object::Integer&>(*e);
      REQUIRE(i.value == tt.expected);
    }
  };
  SECTION("math") {
    struct Test {
      string input;
      int expected;
    };
    std::vector<Test> tests {
        {"5", 5},
        {"10", 10},
        {"-5", -5},
        {"-10", -10},
        {"5 + 5 + 5 + 5 - 10", 10},
        {"2 * 2 * 2 * 2 * 2", 32},
        {"-50 + 100 + -50", 0},
        {"5 * 2 + 10", 20},
        {"5 + 2 * 10", 25},
        {"20 + 2 * -10", 0},
        {"50 / 2 * 2 + 10", 60},
        {"2 * (5 + 10)", 30},
        {"3 * 3 * 3 + 10", 37},
        {"3 * (3 * 3) + 10", 37},
        {"(5 + 10 * 2 + 15 / 3) * 2 + -10", 50},
    };
    for (auto&& tt : tests) {
      auto e = test_eval_node(tt.input);
      auto i = dynamic_cast<object::Integer&>(*e);
      REQUIRE(i.value == tt.expected);
    }
  };
}

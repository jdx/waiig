#include "monkey/parser.h"

#include <fmt/color.h>
#include <fmt/ostream.h>

#include <catch2/catch.hpp>
#include <range/v3/view/enumerate.hpp>

using namespace monkey;
using ranges::views::enumerate;
using std::string;
using std::vector;
using color=fmt::terminal_color;

void check_parser_errors(const Parser& parser) {
  for (auto& err : parser.errors) {
    fmt::print(fmt::fg(color::red), "{}\n", err);
  }
  REQUIRE(parser.errors.size() == 0);
}

void test_let_statement(const Statement& stmt, const string& expected_identifier) {
  REQUIRE(stmt.token_literal() == "let");
  auto& lstmt = dynamic_cast<const LetStatement&>(stmt);
  REQUIRE(lstmt.name->value == expected_identifier);
  REQUIRE(lstmt.name->token_literal() == expected_identifier);
}

TEST_CASE("parser") {
  SECTION("let statements") {
    Lexer lexer{R"(
let x = 5;
let y = 10;
let foobar = 838383;
)"};
    Parser parser{lexer};

    Program program = parser.parse_program();
    check_parser_errors(parser);
    struct Test {
      string expected_identifier;
    };
    auto tests = vector<Test>{
        {"x"},
        {"y"},
        {"foobar"},
    };
    for (auto&& [i, tt] : tests | enumerate) {
      Statement& stmt = *program.statements[i];
      test_let_statement(stmt, tt.expected_identifier);
    }
  };
  SECTION("return statements") {
    Lexer lexer{R"(
return 5;
return 10;
return 993322;
)"};
    Parser parser{lexer};

    Program program = parser.parse_program();
    check_parser_errors(parser);

    REQUIRE(program.statements.size() == 3);
    for (auto& stmt : program.statements) {
      REQUIRE(stmt->token_literal() == "return");
    }
  };
};

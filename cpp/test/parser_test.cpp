#include "monkey/parser.h"

#include <fmt/color.h>
#include <fmt/ostream.h>

#include <catch2/catch.hpp>
#include <range/v3/view/enumerate.hpp>

using namespace monkey;
using namespace fmt::literals;
using ranges::views::enumerate;
using std::make_unique;
using std::string;
using std::vector;
using color = fmt::terminal_color;

void check_parser_errors(const Parser& parser) {
  for (auto& err : parser.errors) {
    fmt::print(fmt::fg(color::red), "{}\n", err);
  }
  REQUIRE(parser.errors.size() == 0);
}

void test_let_statement(const Statement& stmt,
                        const string& expected_identifier) {
  REQUIRE(stmt.token_literal() == "let");
  auto& lstmt = dynamic_cast<const LetStatement&>(stmt);
  REQUIRE(lstmt.name->value == expected_identifier);
  REQUIRE(lstmt.name->token_literal() == expected_identifier);
}

void test_literal_expression(const Expression& exp, const int value) {
  auto& il = dynamic_cast<const IntegerLiteral&>(exp);
  REQUIRE(il.value == value);
  REQUIRE(il.token_literal() == std::to_string(value));
}

void test_literal_expression(const Expression& exp, const string& value) {
  auto& ident = dynamic_cast<const Identifier&>(exp);
  REQUIRE(ident.value == value);
  REQUIRE(ident.token_literal() == value);
}

Program parse(string input) {
  Lexer l{input};
  Parser p{l};
  Program program = p.parse_program();
  check_parser_errors(p);
  return program;
}

void test_parse(string input, string expected) {
  Program program = parse(input);
  REQUIRE("{}"_format(program) == expected);
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

  SECTION("to_str") {
    auto lstmt  = make_unique<LetStatement>(Token{Token::Type::LET, "let"});
    lstmt->name = make_unique<Identifier>(Token{Token::Type::IDENT, "myVar"});
    lstmt->value =
        make_unique<Identifier>(Token{Token::Type::IDENT, "anotherVar"});
    Program p{};
    p.statements.push_back(std::move(lstmt));
    REQUIRE("{}"_format(p) == "let myVar = anotherVar;");
  };

  SECTION("identifier expression") {
    Lexer l{"foobar;"};
    Parser p{l};
    Program program = p.parse_program();
    check_parser_errors(p);
    REQUIRE(program.statements.size() == 1);
    auto& stmt  = dynamic_cast<ExpressionStatement&>(*program.statements[0]);
    auto& ident = dynamic_cast<Identifier&>(*stmt.expression);
    REQUIRE(ident.value == "foobar");
    REQUIRE(ident.token_literal() == "foobar");
  };

  SECTION("integer literal expression") {
    Lexer l{"5;"};
    Parser p{l};
    Program program = p.parse_program();
    check_parser_errors(p);
    REQUIRE(program.statements.size() == 1);
    auto& stmt = dynamic_cast<ExpressionStatement&>(*program.statements[0]);
    auto& lit  = dynamic_cast<IntegerLiteral&>(*stmt.expression);
    REQUIRE(lit.value == 5);
    REQUIRE(lit.token_literal() == "5");
  };

  SECTION("prefix expressions") {
    struct Test {
      string input;
      string op;
      int value;
      string str;
    };
    vector<Test> tests = {
        {"!5;", "!", 5, "(!5)"},
        {"-15;", "-", 15, "(-15)"},
    };
    for (auto tt : tests) {
      Lexer l{tt.input};
      Parser p{l};
      Program program = p.parse_program();
      check_parser_errors(p);
      REQUIRE(program.statements.size() == 1);
      auto& stmt = dynamic_cast<ExpressionStatement&>(*program.statements[0]);
      auto& exp  = dynamic_cast<PrefixExpression&>(*stmt.expression);
      REQUIRE(exp.op == tt.op);
      test_literal_expression(*exp.right, tt.value);
      REQUIRE("{}"_format(program) == tt.str);
    }
  };

  SECTION("infix expressions") {
    struct Test {
      string input;
      int left;
      string op;
      int right;
      string str;
    };
    auto tt         = GENERATE(Test{"5 + 7;", 5, "+", 7, "(5 + 7)"},
                       Test{"5 - 7;", 5, "-", 7, "(5 - 7)"},
                       Test{"5 * 7;", 5, "*", 7, "(5 * 7)"},
                       Test{"5 / 7;", 5, "/", 7, "(5 / 7)"},
                       Test{"5 > 7;", 5, ">", 7, "(5 > 7)"},
                       Test{"5 < 7;", 5, "<", 7, "(5 < 7)"},
                       Test{"5 == 7;", 5, "==", 7, "(5 == 7)"},
                       Test{"5 != 7;", 5, "!=", 7, "(5 != 7)"});
    Program program = parse(tt.input);
    REQUIRE(program.statements.size() == 1);
    auto& stmt = dynamic_cast<ExpressionStatement&>(*program.statements[0]);
    auto& exp  = dynamic_cast<InfixExpression&>(*stmt.expression);
    REQUIRE(exp.op == tt.op);
    test_literal_expression(*exp.left, tt.left);
    test_literal_expression(*exp.right, tt.right);
    REQUIRE("{}"_format(program) == tt.str);
  };

  SECTION("boolean expressions") {
    SECTION("true") {
      Program program = parse("true;");
      REQUIRE(program.statements.size() == 1);
      auto& stmt = dynamic_cast<ExpressionStatement&>(*program.statements[0]);
      auto& exp  = dynamic_cast<Boolean&>(*stmt.expression);
      REQUIRE(exp.value);
      REQUIRE("{}"_format(program) == "true");
    };
    SECTION("false") {
      Program program = parse("false;");
      REQUIRE(program.statements.size() == 1);
      auto& stmt = dynamic_cast<ExpressionStatement&>(*program.statements[0]);
      auto& exp  = dynamic_cast<Boolean&>(*stmt.expression);
      REQUIRE_FALSE(exp.value);
      REQUIRE("{}"_format(program) == "false");
    };
  };
};

TEST_CASE("operator precedence") {
  test_parse("true;", "true");
  test_parse("false;", "false");
  test_parse("3 > 5 == false;", "((3 > 5) == false)");
  test_parse("3 < 5 == true;", "((3 < 5) == true)");
  test_parse("!true;", "(!true)");
};

TEST_CASE("grouped expressions") {
  test_parse("5 + 5 * 2;", "(5 + (5 * 2))");
  test_parse("(5 + 5) * 2", "((5 + 5) * 2)");
  test_parse("1 + (2 + 3) + 4", "((1 + (2 + 3)) + 4)");
  test_parse("2 / (5 + 5)", "(2 / (5 + 5))");
  test_parse("-(5 + 5)", "(-(5 + 5))");
  test_parse("!(true == true)", "(!(true == true))");
}

TEST_CASE("if expression") {
  Program program = parse("if (x < y) { x }");
  REQUIRE(program.statements.size() == 1);
  auto& stmt = dynamic_cast<ExpressionStatement&>(*program.statements[0]);
  auto& exp  = dynamic_cast<IfExpression&>(*stmt.expression);
  REQUIRE("{}"_format(*exp.condition) == "(x < y)");
  REQUIRE("{}"_format(*exp.consequence) == "{ x }");
  REQUIRE(exp.alternative == nullptr);
  REQUIRE("{}"_format(program) == "if (x < y) { x }");
}

TEST_CASE("if expression w/ alternative") {
  Program program = parse("if (x < y) { x } else { y }");
  REQUIRE(program.statements.size() == 1);
  auto& stmt = dynamic_cast<ExpressionStatement&>(*program.statements[0]);
  auto& exp  = dynamic_cast<IfExpression&>(*stmt.expression);
  REQUIRE("{}"_format(*exp.condition) == "(x < y)");
  REQUIRE("{}"_format(*exp.consequence) == "{ x }");
  REQUIRE("{}"_format(*exp.alternative) == "{ y }");
  REQUIRE("{}"_format(program) == "if (x < y) { x } else { y }");
}

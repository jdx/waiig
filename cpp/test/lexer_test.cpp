#include "monkey/lexer.h"

#include <fmt/color.h>
#include <fmt/ostream.h>

#include <catch2/catch.hpp>

using namespace monkey;
using std::string;
using std::vector;

TEST_CASE("lexer") {
  auto test = [](const string& input, const vector<Token>& expected) {
    auto debug = [](const Token& a, const Token& b) {
      using namespace fmt;
      auto c = fg(a == b ? terminal_color::black : terminal_color::red);
      print(c, ".{:>18} = {}\n", a, b);
    };
    Lexer l{input};
    for (size_t i{0}; auto&& expect : expected) {
      auto tok = l.next_token();
      debug(tok, expect);
      REQUIRE(tok.type == expect.type);
      REQUIRE(tok.literal == expect.literal);
      ++i;
    }
  };

  SECTION("1.3") {
    SECTION("empty") {
      string input;
      vector<Token> expected = {
          {Token::Type::EOF_, ""},
      };
      test(input, expected);
    }

    SECTION("simple") {
      string input           = "=+(){},;";
      vector<Token> expected = {
          {Token::Type::ASSIGN, "="},
          {Token::Type::PLUS, "+"},
          {Token::Type::LPAREN, "("},
          {Token::Type::RPAREN, ")"},
          {Token::Type::LBRACE, "{"},
          {Token::Type::RBRACE, "}"},
          {Token::Type::COMMA, ","},
          {Token::Type::SEMICOLON, ";"},
          {Token::Type::EOF_, ""},
      };
      test(input, expected);
    }

    SECTION("less simple") {
      string input           = R"(
let five = 5;
let ten = 10;

let add = fn(x, y) {
  x + y;
};

let result = add(five, ten);
)";
      vector<Token> expected = {
          {Token::Type::LET, "let"},     {Token::Type::IDENT, "five"},
          {Token::Type::ASSIGN, "="},    {Token::Type::INT, "5"},
          {Token::Type::SEMICOLON, ";"}, {Token::Type::LET, "let"},
          {Token::Type::IDENT, "ten"},   {Token::Type::ASSIGN, "="},
          {Token::Type::INT, "10"},      {Token::Type::SEMICOLON, ";"},
          {Token::Type::LET, "let"},     {Token::Type::IDENT, "add"},
          {Token::Type::ASSIGN, "="},    {Token::Type::FUNCTION, "fn"},
          {Token::Type::LPAREN, "("},    {Token::Type::IDENT, "x"},
          {Token::Type::COMMA, ","},     {Token::Type::IDENT, "y"},
          {Token::Type::RPAREN, ")"},    {Token::Type::LBRACE, "{"},
          {Token::Type::IDENT, "x"},     {Token::Type::PLUS, "+"},
          {Token::Type::IDENT, "y"},     {Token::Type::SEMICOLON, ";"},
          {Token::Type::RBRACE, "}"},    {Token::Type::SEMICOLON, ";"},
          {Token::Type::LET, "let"},     {Token::Type::IDENT, "result"},
          {Token::Type::ASSIGN, "="},    {Token::Type::IDENT, "add"},
          {Token::Type::LPAREN, "("},    {Token::Type::IDENT, "five"},
          {Token::Type::COMMA, ","},     {Token::Type::IDENT, "ten"},
          {Token::Type::RPAREN, ")"},    {Token::Type::SEMICOLON, ";"},
          {Token::Type::EOF_, ""},
      };
      test(input, expected);
    }
  }

  SECTION("1.4") {
    string input           = R"(
let five = 5;
let ten = 10;

let add = fn(x, y) {
  x + y;
};

let result = add(five, ten);
!-/*5;
5 < 10 > 5;

if (5 < 10) {
	return true;
} else {
	return false;
}

10 == 10;
10 != 9;
)";
    vector<Token> expected = {
        // clang-format off
        {Token::Type::LET, "let"},
        {Token::Type::IDENT, "five"},
        {Token::Type::ASSIGN, "="},
        {Token::Type::INT, "5"},
        {Token::Type::SEMICOLON, ";"},
        {Token::Type::LET, "let"},
        {Token::Type::IDENT, "ten"},
        {Token::Type::ASSIGN, "="},
        {Token::Type::INT, "10"},
        {Token::Type::SEMICOLON, ";"},
        {Token::Type::LET, "let"},
        {Token::Type::IDENT, "add"},
        {Token::Type::ASSIGN, "="},
        {Token::Type::FUNCTION, "fn"},
        {Token::Type::LPAREN, "("},
        {Token::Type::IDENT, "x"},
        {Token::Type::COMMA, ","},
        {Token::Type::IDENT, "y"},
        {Token::Type::RPAREN, ")"},
        {Token::Type::LBRACE, "{"},
        {Token::Type::IDENT, "x"},
        {Token::Type::PLUS, "+"},
        {Token::Type::IDENT, "y"},
        {Token::Type::SEMICOLON, ";"},
        {Token::Type::RBRACE, "}"},
        {Token::Type::SEMICOLON, ";"},
        {Token::Type::LET, "let"},
        {Token::Type::IDENT, "result"},
        {Token::Type::ASSIGN, "="},
        {Token::Type::IDENT, "add"},
        {Token::Type::LPAREN, "("},
        {Token::Type::IDENT, "five"},
        {Token::Type::COMMA, ","},
        {Token::Type::IDENT, "ten"},
        {Token::Type::RPAREN, ")"},
        {Token::Type::SEMICOLON, ";"},
        {Token::Type::BANG, "!"},
        {Token::Type::MINUS, "-"},
        {Token::Type::SLASH, "/"},
        {Token::Type::ASTERISK, "*"},
        {Token::Type::INT, "5"},
        {Token::Type::SEMICOLON, ";"},
        {Token::Type::INT, "5"},
        {Token::Type::LT, "<"},
        {Token::Type::INT, "10"},
        {Token::Type::GT, ">"},
        {Token::Type::INT, "5"},
        {Token::Type::SEMICOLON, ";"},
        {Token::Type::IF, "if"},
        {Token::Type::LPAREN, "("},
        {Token::Type::INT, "5"},
        {Token::Type::LT, "<"},
        {Token::Type::INT, "10"},
        {Token::Type::RPAREN, ")"},
        {Token::Type::LBRACE, "{"},
        {Token::Type::RETURN, "return"},
        {Token::Type::TRUE, "true"},
        {Token::Type::SEMICOLON, ";"},
        {Token::Type::RBRACE, "}"},
        {Token::Type::ELSE, "else"},
        {Token::Type::LBRACE, "{"},
        {Token::Type::RETURN, "return"},
        {Token::Type::FALSE, "false"},
        {Token::Type::SEMICOLON, ";"},
        {Token::Type::RBRACE, "}"},
        {Token::Type::INT, "10"},
        {Token::Type::EQ, "=="},
        {Token::Type::INT, "10"},
        {Token::Type::SEMICOLON, ";"},
        {Token::Type::INT, "10"},
        {Token::Type::NOT_EQ, "!="},
        {Token::Type::INT, "9"},
        {Token::Type::SEMICOLON, ";"},
        {Token::Type::EOF_, ""},
        // clang-format on
    };
    test(input, expected);
  }
}

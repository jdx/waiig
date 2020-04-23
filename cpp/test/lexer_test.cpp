#include "monkey/lexer.h"

#include <fmt/ostream.h>

#include <catch2/catch.hpp>

using namespace monkey;
using fmt::print;
using std::string;
using std::vector;

TEST_CASE("lexer") {
  auto test = [](const string& input, const vector<Token>& expected) {
    Lexer l{input};
    for (auto&& expect : expected) {
      auto tok = l.next_token();
      // print("{} = {}\n", tok, expect);
      REQUIRE(tok.type == expect.type);
      REQUIRE(tok.literal == expect.literal);
    }
  };

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

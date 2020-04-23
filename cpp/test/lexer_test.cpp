#include "monkey/lexer.h"

#include "init.h"

using namespace monkey;
using std::string;
using std::vector;

TEST_CASE("lexer") {
  auto test = [](const string& input, const vector<Token>& expected) {
    Lexer l{input};
    for (auto&& expect : expected) {
      auto tok = l.next_token();
      REQUIRE(tok.type == expect.type);
      REQUIRE(tok.literal == expect.literal);
    }
  };

  SECTION("empty") {
    string input           = "";
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
}

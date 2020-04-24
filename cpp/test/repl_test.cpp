#include <catch2/catch.hpp>
#include <sstream>

#include "monkey/monkey.h"

using namespace monkey;
using std::istringstream;
using std::ostringstream;

TEST_CASE("repl") {
  SECTION("simple") {
    istringstream in{R"(let add = fn(x, y) { x + y; };)"};
    ostringstream out{};

    repl::start(in, out);

    REQUIRE(out.str() == R"EOF(>> LET<"let">
IDENT<"add">
ASSIGN<"=">
FUNCTION<"fn">
LPAREN<"(">
IDENT<"x">
COMMA<",">
IDENT<"y">
RPAREN<")">
LBRACE<"{">
IDENT<"x">
PLUS<"+">
IDENT<"y">
SEMICOLON<";">
RBRACE<"}">
SEMICOLON<";">
>> 
)EOF");
  };
};

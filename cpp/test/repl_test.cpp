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

    REQUIRE(out.str() == ">> let add = fn(x, y) { (x + y) };\n>> \n");
  };
};

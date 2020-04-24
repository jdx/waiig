#include <catch2/catch.hpp>
#include <sstream>

#include "monkey/monkey.h"

using namespace monkey;
using std::stringstream;

TEST_CASE("repl") {
  SECTION("simple") {
    stringstream in{R"(let add = fn(x, y) { x + y; };)"};
    stringstream out{};

    repl::start(in, out);

    REQUIRE(out.str() == "FOO");
  };
};
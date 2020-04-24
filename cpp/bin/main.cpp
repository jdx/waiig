#include <fmt/ostream.h>
#include <monkey/monkey.h>

#include <iostream>

#include "user.h"

using fmt::print;
using std::string;

const auto VERSION = "0.01";

int main() {
  User user{};

  print(R"(Hello {}! This is the Monkey programming language!
monkey/{} darwin-x64 cpp
)",
        user.name(),
        VERSION);

  monkey::Repl::start(std::cin, std::cout);

  return 0;
}

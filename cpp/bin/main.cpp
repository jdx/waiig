#include <monkey/lexer.h>

#include <iostream>

int main() {
  std::cout << "Hello," << monkey::lex("a") << std::endl;
  return 0;
}

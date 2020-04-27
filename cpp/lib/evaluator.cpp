#include "monkey/evaluator.h"

#include <fmt/ostream.h>

#include <exception>
#include <iostream>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <variant>

namespace monkey {

using std::function;
using std::make_unique;
using std::type_index;
using std::unique_ptr;
using std::unordered_map;
using std::vector;
using ObjPtr = unique_ptr<object::Object>;

ObjPtr eval_statements(const vector<unique_ptr<Statement>>& stmts) {
  ObjPtr result{};
  for (auto& stmt : stmts) {
    result = eval(*stmt);
  }
  return result;
}

ObjPtr eval(const Program& program) {
  fmt::print("evalprog.\n");
  return eval_statements(program.statements);
}

ObjPtr eval(const ExpressionStatement& estmt) {
  fmt::print("evalexp.\n");
  return eval(*estmt.expression);
}

ObjPtr eval(const IntegerLiteral& lit) {
  fmt::print("evallit.\n");
  return make_unique<object::Integer>(lit.value);
}

ObjPtr eval(const Boolean& b) {
  fmt::print("evalb.\n");
  return make_unique<object::Boolean>(b.value);
}

ObjPtr eval(const Node& node) {
  fmt::print("eval.\n");

  static unordered_map<type_index, function<ObjPtr(const Node&)>> lookup{
      {typeid(Program), [](const Node& node){ return eval(dynamic_cast<const Program&>(node)); }},
      {typeid(ExpressionStatement), [](const Node& node){ return eval(dynamic_cast<const ExpressionStatement&>(node)); }},
      {typeid(IntegerLiteral), [](const Node& node){ return eval(dynamic_cast<const IntegerLiteral&>(node)); }},
      {typeid(Boolean), [](const Node& node){ return eval(dynamic_cast<const Boolean&>(node)); }},
  };
  return lookup[typeid(node)](node);
}

} // namespace monkey
#include "monkey/object.h"

#include <string>
#include <unordered_map>

namespace monkey::object {

using std::unordered_map;

Integer::Integer(int val)
    : value{val} { }

const Type& Integer::type() {
  return types::INTEGER;
}

std::string Integer::Inspect() {
  return std::to_string(value);
}

Boolean::Boolean(bool value)
    : value{value} { }

const Type& Boolean::type() {
  return types::BOOLEAN;
}

std::string Boolean::Inspect() {
  return std::to_string(value);
}

const Type& Null::type() {
  return types::NULL_;
}

std::string Null::Inspect() {
  return "null";
}

} // namespace monkey::object

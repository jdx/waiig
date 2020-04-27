#include "monkey/object.h"

#include <string>
#include <unordered_map>

namespace monkey::object {

using std::unordered_map;
using std::ostream;

ostream& operator<<(ostream& out, const Object& obj) {
  return out << obj.inspect();
}

Integer::Integer(int val)
    : value{val} { }

const Type& Integer::type() {
  return types::INTEGER;
}

std::string Integer::inspect() const {
  return std::to_string(value);
}

Boolean::Boolean(bool value)
    : value{value} { }

const Type& Boolean::type() {
  return types::BOOLEAN;
}

std::string Boolean::inspect() const {
  return value ? "true" : "false";
}

const Type& Null::type() {
  return types::NULL_;
}

std::string Null::inspect() const {
  return "null";
}

} // namespace monkey::object

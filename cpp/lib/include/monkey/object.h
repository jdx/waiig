#pragma once
#include <ostream>
#include <string>

namespace monkey::object {

typedef std::string Type;

namespace types {
  const Type NULL_   = "NULL";
  const Type INTEGER = "INTEGER";
  const Type BOOLEAN = "BOOLEAN";
} // namespace types

struct Object {
  virtual ~Object()             = default;
  virtual const Type& type()    = 0;
  virtual std::string Inspect() = 0;
};

struct Integer : Object {
  int value;

  explicit Integer(int value);
  const Type& type() override;
  std::string Inspect() override;

};

struct Boolean : Object {
  bool value;

  explicit Boolean(bool value);
  const Type& type() override;
  std::string Inspect() override;
};

struct Null : Object {
  const Type& type() override;
  std::string Inspect() override;
};

} // namespace monkey::object

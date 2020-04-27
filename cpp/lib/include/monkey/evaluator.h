#pragma once
#include "ast.h"
#include "object.h"

namespace monkey {

std::unique_ptr<object::Object> eval(const Node& node);

}

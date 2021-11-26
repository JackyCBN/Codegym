#include <fmt/format.h>
#include "greeter/greeter.h"

using namespace YoutaoEngine;

greeter::greeter(std::string _name) : name(std::move(_name)) {}

std::string greeter::greet() const {
  return fmt::format("Hello, {}!", name);
}

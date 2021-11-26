#include <doctest/doctest.h>
#include <greeter/greeter.h>
#include <youtaoengine/version.h>

#include <string>

TEST_CASE("YoutaoEngine") {
  using namespace YoutaoEngine;

  greeter greeter("Tests");

  CHECK(greeter.greet() == "Hello, Tests!");
}

TEST_CASE("YoutaoEngine version") {
  static_assert(std::string_view(YOUTAOENGINE_VERSION) == std::string_view("1.0"));
  CHECK(std::string(YOUTAOENGINE_VERSION) == std::string("1.0"));
}

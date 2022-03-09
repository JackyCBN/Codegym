#include <greeter/greeter.h>
#include <youtaoengine/version.h>

#include <cxxopts.hpp>
#include <iostream>
#include <string>
#include <unordered_map>

auto main(int argc, char** argv) -> int {

  cxxopts::Options options(*argv, "A program to welcome the world!");

  std::string name;

  // clang-format off
  options.add_options()
    ("h,help", "Show help")
    ("v,version", "Print the current version number")
    ("n,name", "Name to greet", cxxopts::value(name)->default_value("Youtao Game Engine"))
  ;
  // clang-format on

  auto result = options.parse(argc, argv);

  if (result["help"].as<bool>()) {
    std::cout << options.help() << std::endl;
    return 0;
  }

  if (result["version"].as<bool>()) {
    std::cout << "YoutaoEngine, version " << YOUTAOENGINE_VERSION << std::endl;
    return 0;
  }

  YoutaoEngine::greeter greeter(name);
  std::cout << greeter.greet() << std::endl;

  return 0;
}

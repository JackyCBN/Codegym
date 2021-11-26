#pragma once

#include <string>

namespace YoutaoEngine {

  /**
   * @brief A class for saying hello in multiple languages
   */
  class greeter {
    std::string name;

  public:
    /**
     * @brief Creates a new greeter
     * @param name the name to greet
     */
    greeter(std::string name);

    /**
     * @brief Creates a localized string containing the greeting
     * @return a string containing the greeting
     */
    std::string greet() const;
  };

}  // namespace YoutaoEngine

CPMAddPackage(
  NAME rttr # link against RTTR::Core_Lib
  GIT_TAG 7edbd580cfad509a3253c733e70144e36f02ecd4
  GITHUB_REPOSITORY rttrorg/rttr
  OPTIONS
    "BUILD_RTTR_DYNAMIC Off"
    "BUILD_UNIT_TESTS Off"
    "BUILD_STATIC On"
    "BUILD_PACKAGE Off"
    "BUILD_WITH_RTTI On"
    "BUILD_EXAMPLES Off"
    "BUILD_DOCUMENTATION Off"
    "BUILD_INSTALLER Off"
    "USE_PCH Off"
    "CUSTOM_DOXYGEN_STYLE Off"
)

CPMAddPackage(
  NAME EnTT
  VERSION 3.9.0
  GITHUB_REPOSITORY skypjack/entt
  # EnTT's CMakeLists screws with configuration options
  DOWNLOAD_ONLY True
)

if(EnTT_ADDED)
  add_library(EnTT INTERFACE)
  target_include_directories(EnTT SYSTEM INTERFACE ${EnTT_SOURCE_DIR}/src)
endif()


CPMAddPackage(
  NAME cereal
  VERSION 1.3.2
  GITHUB_REPOSITORY USCiLab/cereal
  OPTIONS "SKIP_PORTABILITY_TEST ON" "JUST_INSTALL_CEREAL ON"
)

CPMAddPackage("gh:SFML/SFML#2.5.1")
CPMAddPackage("gh:ocornut/imgui#docking")

#[[ CPMAddPackage(
    GITHUB_REPOSITORY "eliasdaler/imgui-sfml"
    VERSION "2.5"
    OPTIONS "IMGUI_DIR ${imgui_SOURCE_DIR}"
)
add_compile_definitions(IMGUI_DISABLE_DEMO_WINDOWS) # imgui-sfml doesn't compile demo by default
]]
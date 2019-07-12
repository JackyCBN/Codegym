@echo off

REM CMAKE_BUILD_TYPE not being used in CMakeLists.txt (https://stackoverflow.com/questions/24460486/cmake-build-type-not-being-used-in-cmakelists-txt)
cmake -H. -B_builds -DCMAKE_BUILD_TYPE=Debug "-GVisual Studio 15 2017 Win64"
cmake -H. -B_builds -DCMAKE_BUILD_TYPE=Release  "-GVisual Studio 15 2017 Win64"

cmake --build _builds --config Debug
cmake --build _builds --config Release


pause


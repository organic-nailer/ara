# CMakeList.txt : CMake project for ara, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (ara "src/ara.cpp" "include/ara.h" "include/html_tokenizer.h" "src/html_tokenizer.cpp" "include/util.h")

# TODO: Add tests and install targets if needed.
target_include_directories(ara PRIVATE "include")

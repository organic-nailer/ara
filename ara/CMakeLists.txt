# CMakeList.txt : CMake project for ara, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (ara "src/ara.cpp" "include/ara.h" "include/html_tokenizer.h" "src/html_tokenizer.cpp" "include/util.h" "include/dom_generator.h" "src/dom_generator.cpp" "src/ara_window.cpp")

# TODO: Add tests and install targets if needed.
target_include_directories(ara PRIVATE "include")

include_directories("C:/src/skia/skia")
include_directories("C:/src/skia/skia/tools/sk_app")
#include_directories("C:/src/glfw-3.3.2/include")
link_directories("C:/src/skia/skia/out/Debug")
#link_directories("C:/src/glfw-3.3.2/lib")
#target_link_libraries(ara INTERFACE glfw3)
target_link_libraries(ara opengl32)
target_link_libraries(ara "C:/src/skia/skia/out/Debug/skia.lib")

add_subdirectory("third/glfw-3.3.2")
include_directories("third/glfw/include")
target_link_libraries(ara glfw)

set_property(TARGET ara PROPERTY CXX_STANDARD 17)

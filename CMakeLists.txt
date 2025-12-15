cmake_minimum_required(VERSION 3.26)
project(gravity-sim)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS OFF)
set(USE_WAYLAND ON)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

add_subdirectory(lib/raylib)
add_subdirectory(src)
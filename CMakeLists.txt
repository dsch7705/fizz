cmake_minimum_required(VERSION 3.26)
project(gravity-sim)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(ASAN OFF)
if(ASAN)
  add_compile_options(-fsanitize=address)
  add_link_options(-fsanitize=address)
endif()

add_subdirectory(lib/raylib)
add_subdirectory(src)

add_executable(fizz main.cpp)
target_include_directories(fizz PRIVATE include)
target_link_libraries(fizz PRIVATE Fizz::Core Fizz::Constraint Fizz::Systems raylib)
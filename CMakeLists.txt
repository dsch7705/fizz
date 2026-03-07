cmake_minimum_required(VERSION 3.26)
project(fizz)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(ASAN OFF)
if(ASAN)
  add_compile_options(-fsanitize=address)
  add_link_options(-fsanitize=address)
endif()

add_subdirectory(src)

if(BUILD_FIZZ_EXAMPLES)
  add_subdirectory(examples)
endif()
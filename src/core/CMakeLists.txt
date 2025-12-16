set(CORE_SRC 
  System.cpp
  Body.cpp
  DVec.cpp
  Constraint.cpp
  Util.cpp)

add_library(fizz-core ${CORE_SRC})
target_include_directories(fizz-core PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_library(Fizz::Core ALIAS fizz-core)
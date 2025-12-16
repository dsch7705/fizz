set(SYSTEMS_SRC
  Pendulum.cpp)

add_library(fizz-systems ${SYSTEMS_SRC})
target_include_directories(fizz-systems PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(fizz-systems PUBLIC Fizz::Core Fizz::Constraint)

add_library(Fizz::Systems ALIAS fizz-systems)
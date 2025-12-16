set(CONSTRAINT_SRC
  DistanceConstraint.cpp
  SpringConstraint.cpp
  PositionConstraint.cpp)

add_library(fizz-constraint ${CONSTRAINT_SRC})
target_include_directories(fizz-constraint PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(fizz-constraint PUBLIC Fizz::Core)

add_library(Fizz::Constraint ALIAS fizz-constraint)
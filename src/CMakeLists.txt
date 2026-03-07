set(FIZZ_SRC
  Body.cpp
  Constraint.cpp
  DVec.cpp
  System.cpp
  Util.cpp
  Draw.cpp

  constraints/DistanceConstraint.cpp
  constraints/SpringConstraint.cpp
  constraints/PositionConstraint.cpp
  
  ../python/PyAPI.cpp
)

add_library(fizz ${FIZZ_SRC})
target_include_directories(fizz PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_custom_command(
  TARGET fizz
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_BINARY_DIR}/libfizz.so
    ${FIZZ_PYTHON_DIR}/libfizz.so
  COMMENT "Copying libfizz.so to ${FIZZ_PYTHON_DIR}"
)
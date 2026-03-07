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
  
  PyAPI.cpp
)

add_library(fizz ${FIZZ_SRC})
target_include_directories(fizz PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
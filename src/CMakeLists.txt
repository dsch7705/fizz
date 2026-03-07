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

if(MSVC)
  set_property(TARGET fizz PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif(MSVC)

if(BUILD_SHARED_LIBS)  
  add_custom_command(
    TARGET fizz
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
      "$<TARGET_FILE:fizz>"
      ${FIZZ_PYTHON_DIR}/
    COMMENT "Copying fizz shared library to ${FIZZ_PYTHON_DIR}"
  )
endif(BUILD_SHARED_LIBS)

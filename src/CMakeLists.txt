set(SRC main.cpp
        dvec.cpp
        util.cpp
        solver.cpp
        system.cpp)

add_executable(fizz ${SRC})

target_include_directories(fizz PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(fizz PRIVATE raylib)
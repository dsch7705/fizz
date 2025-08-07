set(SRC main.cpp
        dvec.cpp
        solver.cpp
        systems.cpp)

add_executable(gravity ${SRC})

target_include_directories(gravity PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(gravity PRIVATE raylib)
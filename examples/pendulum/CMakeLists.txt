add_executable(pendulum main.cpp Pendulum.cpp)
target_link_libraries(pendulum PRIVATE fizz SDL3::SDL3)
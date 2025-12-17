include(FetchContent)

FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
)
FetchContent_MakeAvailable(raylib)

add_subdirectory(pendulum)
add_subdirectory(pogo)
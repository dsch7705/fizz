add_subdirectory(SDL)

add_library(ImGui STATIC 
  imgui/imgui.cpp
  imgui/imgui_draw.cpp
  imgui/imgui_tables.cpp
  imgui/imgui_widgets.cpp
  imgui/backends/imgui_impl_sdl3.cpp
  imgui/backends/imgui_impl_sdlrenderer3.cpp
)
target_link_libraries(ImGui PUBLIC SDL3::SDL3)
target_include_directories(ImGui PUBLIC imgui)
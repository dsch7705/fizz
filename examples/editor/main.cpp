#include "../SDL_draw_impl.h"

#include "Editor.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"
#include "fizz/System.h"
#include "imgui.h"

#include <iostream>

SDL_Window* sdl_window = nullptr;
SDL_Renderer* sdl_renderer = nullptr;

int main()
{
  Draw::Transform& transform = Draw::getTransform();
  transform.scale = 10.f;

  // SDL setup
  if (!sdl_setup("Editor")) {
    return -1;
  }

  // fizz setup
  Draw::setCircleCallback(sdl_circle);
  Draw::setLineCallback(sdl_line);

  System system;
  ID b0 = system.createBody(Vec2{0, 0}, 0.2, true);
  ID b1 = system.createBody(Vec2{1, 0}, 0.2);
  system.createConstraint<DistanceConstraint>(b0, b1);

  // ImGui setup
  ImGui::CreateContext();
  ImGui_ImplSDL3_InitForSDLRenderer(sdl_window, sdl_renderer);
  ImGui_ImplSDLRenderer3_Init(sdl_renderer);
  ImGuiIO& io = ImGui::GetIO();

  // Editor editor{};

  bool running = true;
  SDL_Event event;
  while (running) {
    float dt = calc_dt();

    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL3_ProcessEvent(&event);
      process_sdl_event(event);

      switch (event.type) {
        case SDL_EVENT_QUIT:
          running = false;
          break;

        case SDL_EVENT_KEY_DOWN:
          if (event.key.key == SDLK_SPACE) {
            Editor::Popup<Editor::PopupID::CreateBody>::open();
          }
          break;

        default:
          break;
      }
    }

    system.update(dt);

    // Rendering
    SDL_SetRenderDrawColor(sdl_renderer, 255, 255, 255, 255);
    SDL_RenderClear(sdl_renderer);

    Draw::grid(3.f, {255, 0, 255, 32});
    system.draw({0, 0, 0, 255});

    // ImGui
    ImGui_ImplSDL3_NewFrame();
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Editor");
    Editor::render();
    ImGui::End();
    // editor.createBodyPopup();

    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), sdl_renderer);

    SDL_RenderPresent(sdl_renderer);
  }

  sdl_cleanup();
  return 0;
}
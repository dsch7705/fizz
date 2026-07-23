#include "../SDL_Draw_impl.h"

#include "fizz/System.h"

#include <iostream>

SDL_Renderer* sdl_renderer = nullptr;
SDL_Window* sdl_window = nullptr;

int main()
{
  // SDL setup
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cout << "Failed to initialize SDL";
    return -1;
  }
  SDL_CreateWindowAndRenderer("Debugger", 640, 480, 0, &sdl_window, &sdl_renderer);
  SDL_SetRenderVSync(sdl_renderer, 1);

  // fizz setup
  Draw::setCircleCallback(sdl_circle);
  Draw::setLineCallback(sdl_line);

  System system;
  ID b0 = system.createBody(Vec2{0, 0}, 0.2, true);
  ID b1 = system.createBody(Vec2{10, 0}, 0.2);
  system.createConstraint<DistanceConstraint>(b0, b1);

  Draw::Transform& transform = Draw::getTransform();
  transform.scale = 10;
  transform.offset = Vec2{5, 2};

  float dt = 0.f;
  uint64_t lastPerfCounter = SDL_GetPerformanceCounter();
  uint64_t perfFreq = SDL_GetPerformanceFrequency();

  bool running = true;
  SDL_Event event;
  while (running) {
    dt = calc_dt();

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
      }
    }

    system.update(dt);

    // Rendering
    SDL_SetRenderDrawColor(sdl_renderer, 0, 255, 255, 255);
    SDL_RenderClear(sdl_renderer);

    system.draw({0, 0, 0, 255});

    SDL_RenderPresent(sdl_renderer);
  }

  SDL_DestroyRenderer(sdl_renderer);
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();

  return 0;
}
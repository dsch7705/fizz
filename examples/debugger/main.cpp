#include "../SDL_draw_impl.h"

#include "fizz/System.h"

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

  float dt = 0.f;
  uint64_t lastPerfCounter = SDL_GetPerformanceCounter();
  uint64_t perfFreq = SDL_GetPerformanceFrequency();

  SDL_Texture* tex = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1, 1);
  SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
  SDL_SetRenderTarget(sdl_renderer, tex);
  SDL_SetRenderDrawColor(sdl_renderer, 255, 255, 255, 255);
  SDL_RenderClear(sdl_renderer);
  SDL_SetRenderTarget(sdl_renderer, nullptr);

  bool running = true;
  SDL_Event event;
  while (running) {
    dt = calc_dt();

    while (SDL_PollEvent(&event)) {
      process_sdl_event(event);

      if (event.type == SDL_EVENT_QUIT) {
        running = false;
        break;
      }
    }

    system.update(dt);

    // Rendering
    SDL_SetRenderDrawColor(sdl_renderer, 255, 255, 255, 255);
    SDL_RenderClear(sdl_renderer);

    Draw::grid(3.f, {255, 0, 255, 32});
    system.draw({0, 0, 0, 255});

    SDL_RenderPresent(sdl_renderer);
  }

  sdl_cleanup();
  return 0;
}
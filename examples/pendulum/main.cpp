#include "Pendulum.h"

#include "fizz/Draw.h"

#include "../SDL_draw_impl.h"

SDL_Window* sdl_window;
SDL_Renderer* sdl_renderer;

int main(int argc, char** argv)
{
  constexpr int screenW = 640;
  constexpr int screenH = 480;

  if (!sdl_setup(screenW, screenH)) {
    return -1;
  }

  Draw::Transform& transform = Draw::getTransform();
  transform.scale = 50;
  transform.offset = Vec2(screenW, screenH) / transform.scale / 2;

  int links = 2;
  double distance = screenH / transform.scale / 2 / links * 0.9;
  Pendulum p(2, Vec2(0, 0), distance, false);

  SDL_Event event;
  bool running = true;
  while (running) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_EVENT_QUIT:
          running = false;
          break;

        default:
          break;
      }
    }

    p.update(calc_dt());

    SDL_SetRenderDrawColor(sdl_renderer, 255, 255, 255, 255);
    SDL_RenderClear(sdl_renderer);

    p.draw({0, 0, 0, 255});

    SDL_RenderPresent(sdl_renderer);
  }

  sdl_cleanup();
  return 0;
}
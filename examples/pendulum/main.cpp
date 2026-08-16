#include "Pendulum.h"

#include "SDL3/SDL_render.h"
#include "fizz/Draw.h"

#include "../SDL_draw_impl.h"

SDL_Window* sdl_window;
SDL_Renderer* sdl_renderer;

int main(int argc, char** argv)
{
  Draw::Transform& transform = Draw::getTransform();
  transform.scale = 300.f;

  if (!sdl_setup("Pendulum")) {
    return -1;
  }

  int links = 2;
  double distance = 0.2f;  ////transform.screenSize.y / transform.scale / 2 / links * 0.9;
  Pendulum p(2, Vec2(0, 0.5f), distance);
  SDL_Event event;
  bool running = true;
  while (running) {
    while (SDL_PollEvent(&event)) {
      process_sdl_event(event);

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

    Draw::grid(1.f, {0, 0, 0, 32});
    p.draw({0, 0, 0, 255});

    SDL_RenderPresent(sdl_renderer);
  }

  sdl_cleanup();
  return 0;
}
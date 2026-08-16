#include "Pendulum.h"

#include "SDL3/SDL_render.h"
#include "fizz/Draw.h"

#include "../SDL_draw_impl.h"

SDL_Window* sdl_window;
SDL_Renderer* sdl_renderer;

int main(int argc, char** argv)
{
  unsigned int screenW = 640;
  unsigned int screenH = 480;

  if (!sdl_setup(screenW, screenH)) {
    return -1;
  }

  Draw::Transform& transform = Draw::getTransform();
  transform.scale = 50;
  transform.center(screenW, screenH);

  int links = 2;
  double distance = screenH / transform.scale / 2 / links * 0.9;
  Pendulum p(2, Vec2(0, 0), distance);
  SDL_Event event;
  bool running = true;
  while (running) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_EVENT_QUIT:
          running = false;
          break;

        case SDL_EVENT_KEY_DOWN:
          if (event.key.key == SDLK_UP) {
            ++transform.scale;
            transform.center(screenW, screenH);
          }
          else if (event.key.key == SDLK_DOWN) {
            --transform.scale;
            transform.center(screenW, screenH);
          }

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
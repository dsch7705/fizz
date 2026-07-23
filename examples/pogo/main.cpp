#include "fizz/Constants.h"
#include "fizz/System.h"
#include "fizz/constraints/PositionConstraint.h"
#include "fizz/constraints/SpringConstraint.h"

#include "../SDL_draw_impl.h"

SDL_Window* sdl_window;
SDL_Renderer* sdl_renderer;

int main(int argc, char** argv)
{
  constexpr int screenW = 640;
  constexpr int screenH = 480;

  sdl_setup(screenW, screenH);

  Draw::Transform& transform = Draw::getTransform();
  transform.scale = 50;
  transform.offset = Vec2(screenW, screenH) / transform.scale / 2;

  Vec2 worldSize = Vec2(screenW, screenH) / transform.scale;

  System system;
  ID b0 = system.createBody({0, -worldSize.y / 2}, 0.5, false, 1.0);
  ID b1 = system.createBody({0, 0}, 0.5, false, 1.0);
  system.createConstraint<SpringConstraint>(b0, b1);
  system.createConstraint<PositionConstraint>(Vec2(-worldSize.x / 2, -worldSize.y * 2), worldSize / 2, 0.6, b0, b1);

  SDL_Event event;
  bool running = true;
  bool space_held = false;
  while (running) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_EVENT_QUIT:
          running = false;
          break;

        case SDL_EVENT_KEY_DOWN:
          if (event.key.key == SDLK_SPACE) {
            space_held = true;
          }
          break;

        case SDL_EVENT_KEY_UP:
          if (event.key.key == SDLK_SPACE) {
            space_held = false;
          }
          break;

        default:
          break;
      }
    }

    if (space_held) {
      system.getBody(b0).addForce(Vec2(0.0, 250.0));
    }

    system.update(calc_dt());

    SDL_SetRenderDrawColor(sdl_renderer, 255, 255, 255, 255);
    SDL_RenderClear(sdl_renderer);

    system.draw({0, 0, 0, 255});

    SDL_RenderPresent(sdl_renderer);
  }

  sdl_cleanup();
}
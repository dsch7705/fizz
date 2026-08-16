#include "SDL3/SDL_events.h"
#include "fizz/Constants.h"
#include "fizz/Draw.h"
#include "fizz/System.h"
#include "fizz/constraints/DistanceConstraint.h"
#include "fizz/constraints/SpringConstraint.h"

// #include "../raylib_Draw.h"
#include "../SDL_draw_impl.h"

#include <iostream>
#include <string>
#include <vector>

bool intersects(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& d)
{
  auto orient = [](const Vec2& p, const Vec2& q, const Vec2& r) {
    return (q.x - p.x) * (r.y - p.y) - (q.y - p.y) * (r.x - p.x);
  };

  double o1 = orient(a, b, c);
  double o2 = orient(a, b, d);
  double o3 = orient(c, d, a);
  double o4 = orient(c, d, b);

  return (o1 * o2 < 0.0) && (o3 * o4 < 0.0);
}

void grid(System& system)
{
  constexpr double size = 0.4;
  constexpr int w = 100;
  constexpr int h = 75;
  const Vec2 offset(w * -size / 2, h * size * 0.8);

  constexpr double k = 1000.0;
  constexpr double damping = -1.0;

  constexpr int anchorW = 5;

  std::vector<ID> row, lastRow;
  ID lastBody = -1;
  for (int i_y = 0; i_y < h; i_y++) {
    ID lastB = -1;
    for (int i_x = 0; i_x < w; i_x++) {
      bool isAnchor = !(i_x % 7) && (i_y == 0);

      ID b = system.createBody(Vec2(offset.x + i_x * size, offset.y + i_y * size), 0.f, isAnchor, 0.01f);
      Body& body = system.getBody(b);
      body.isVisible = false;

      row.push_back(b);
      float c_width = 0.1f;
      if (i_x < lastRow.size()) {
        system.createConstraint<SpringConstraint>(b, lastRow[i_x], k, damping, c_width);
      }
      if (lastB != -1) {
        system.createConstraint<SpringConstraint>(b, lastB, k, damping, c_width);
      }

      lastB = b;
    }

    lastRow = std::move(row);
    lastBody = lastB;
  }
}

SDL_Window* sdl_window = nullptr;
SDL_Renderer* sdl_renderer = nullptr;

int main(int argc, char** argv)
{
  Draw::Transform& transform = Draw::getTransform();
  transform.scale = 7.f;

  if (!sdl_setup("Cloth")) {
    return -1;
  }

  System system;
  grid(system);

  Vec2 lastMouse;
  std::vector<ID> toDelete;

  bool running = true;
  bool mouseDown = false;

  SDL_Event event;
  while (running) {
    while (SDL_PollEvent(&event)) {
      process_sdl_event(event);

      switch (event.type) {
        case SDL_EVENT_QUIT:
          running = false;
          break;

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
          if (event.button.button == SDL_BUTTON_LEFT) {
            mouseDown = true;
          }
          break;

        case SDL_EVENT_MOUSE_BUTTON_UP:
          if (event.button.button == SDL_BUTTON_LEFT) {
            mouseDown = false;
          }
          break;

        case SDL_EVENT_KEY_DOWN:
          switch (event.key.key) {
            case SDLK_R:
              system.clear();
              grid(system);
              break;
          }
          break;

        default:
          break;
      }
    }

    Vec2 mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);
    mouse = Draw::screenToWorld(mouse);

    if (mouseDown) {
      for (auto& constraint : system.constraints()) {
        auto& s = std::get<SpringConstraint>(constraint);
        const auto& b0 = system.getBody(s.b0());
        const auto& b1 = system.getBody(s.b1());

        if (intersects(mouse, lastMouse, b0.pos(), b1.pos())) {
          toDelete.push_back(s.id());
        }
      }

      for (ID id : toDelete) {
        system.removeConstraint(id);
      }
      toDelete.clear();
    }

    // zoom(transform, GetMouseWheelMove() * (IsKeyDown(KEY_LEFT_SHIFT) ? 0.5f : 1.f));

    system.update(calc_dt());

    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
    SDL_RenderClear(sdl_renderer);

    system.draw(Draw::Color{255, 255, 255, 255});

    SDL_RenderPresent(sdl_renderer);

    lastMouse = mouse;
  }

  sdl_cleanup();
  return 0;
}

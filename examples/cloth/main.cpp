#include "fizz/Constants.h"
#include "fizz/Draw.h"
#include "fizz/System.h"
#include "fizz/constraints/DistanceConstraint.h"
#include "fizz/constraints/SpringConstraint.h"

#include "../raylib_Draw.h"

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
  const Vec2 offset(w * size / 2, h * size * 0.8);

  constexpr double k = 50000.0;
  constexpr double damping = -1.0;

  constexpr int anchorW = 5;

  std::vector<ID> row, lastRow;
  ID lastBody = -1;
  for (int i_y = 0; i_y < h; i_y++) {
    ID lastB = -1;
    for (int i_x = 0; i_x < w; i_x++) {
      bool isAnchor = !(i_x % 7) && (i_y == 0);

      ID b = system.createBody(Vec2(-offset.x + i_x * size, -offset.y + i_y * size), 0.02, isAnchor);
      row.push_back(b);
      if (i_x < lastRow.size()) {
        system.createConstraint<SpringConstraint>(b, lastRow[i_x], k, damping);
      }
      if (lastB != -1) {
        system.createConstraint<SpringConstraint>(b, lastB, k, damping);
      }

      lastB = b;
    }

    lastRow = std::move(row);
    lastBody = lastB;
  }
}

constexpr int screenW = 640;
constexpr int screenH = 480;

void zoom(Draw::Transform& t, float val)
{
  t.scale += val;
  t.offset = Vec2(screenW, screenH) / t.scale / 2.f;
  // t.offset -= val * 2;
}

int main(int argc, char** argv)
{
  Draw::setCircleCallback(raylib_circle);
  Draw::setLineCallback(raylib_line);

  Draw::Transform& transform = Draw::getTransform();
  transform.scale = 7;
  transform.offset = Vec2(screenW, screenH) / transform.scale / 2;

  System system;
  grid(system);

  InitWindow(screenW, screenH, "Cloth");
  SetTargetFPS(60);

  Vec2 lastMouse;
  std::vector<ID> toDelete;
  while (!WindowShouldClose()) {
    Vec2 mouse(GetMouseX(), GetMouseY());
    mouse = Draw::screenToWorld(mouse);

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
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

    if (IsKeyPressed(KEY_R)) {
      system.clear();
      grid(system);
    }

    zoom(transform, GetMouseWheelMove() * (IsKeyDown(KEY_LEFT_SHIFT) ? 0.5f : 1.f));

    system.update(GetFrameTime());

    BeginDrawing();

    ClearBackground(BLACK);
    system.draw(Draw::Color{255, 255, 255, 255});

    DrawText(std::to_string(GetFPS()).c_str(), 5, 5, 30, GREEN);

    EndDrawing();

    lastMouse = mouse;
  }
}

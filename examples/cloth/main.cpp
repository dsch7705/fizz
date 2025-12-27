#include "fizz/Constants.h"
#include "fizz/Draw.h"
#include "fizz/System.h"
#include "fizz/constraints/DistanceConstraint.h"
#include "fizz/constraints/SpringConstraint.h"

#include "../raylib_Draw.h"

#include <vector>

bool intersects(const DVec2& a, const DVec2& b, const DVec2& c, const DVec2& d)
{
  auto orient = [](const DVec2& p, const DVec2& q, const DVec2& r) {
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
  constexpr double size = 0.2;
  constexpr int w = 71;
  constexpr int h = 35;
  constexpr DVec2 offset(kMetersWidth / 2.0 - (w * size) / 2.0, 1.0);

  constexpr double k = 500000.0;
  constexpr double damping = -1.0;

  constexpr int anchorW = 5;

  std::vector<Body*> row, lastRow;
  Body* lastBody = nullptr;
  for (int i_y = 0; i_y < h; i_y++) {
    Body* lastB = nullptr;
    for (int i_x = 0; i_x < w; i_x++) {
      bool isAnchor = !(i_x % 7) && (i_y == 0);

      Body* b = system.createBody(DVec2(offset.x + i_x * size, offset.y + i_y * size), 0.025, isAnchor);
      row.push_back(b);
      if (i_x < lastRow.size()) {
        system.createConstraint<SpringConstraint>(b, lastRow[i_x], k, damping);
      }
      if (lastB) {
        system.createConstraint<SpringConstraint>(b, lastB, k, damping);
      }

      lastB = b;
    }

    lastRow = std::move(row);
    lastBody = lastB;
  }
}

int main(int argc, char** argv)
{
  Draw::setCircleFunc(raylib_circle);
  Draw::setLineFunc(raylib_line);

  System system;
  grid(system);

  InitWindow(kScreenWidth, kScreenHeight, "Cloth");
  SetTargetFPS(60);

  DVec2 lastMouse;
  std::vector<Constraint*> toDelete;
  while (!WindowShouldClose()) {
    DVec2 mouse(GetMouseX(), GetMouseY());
    mouse = Util::screenToWorld(mouse);

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      for (auto& [id, constraint] : system.constraints()) {
        SpringConstraint* s = dynamic_cast<SpringConstraint*>(constraint.get());
        if (!s)
          continue;

        if (intersects(mouse, lastMouse, s->b0()->pos(), s->b1()->pos())) {
          toDelete.push_back(s);
        }
      }

      for (Constraint* c : toDelete) {
        system.removeConstraint(c->id());
      }
      toDelete.clear();
    }

    if (IsKeyPressed(KEY_R)) {
      system.clear();
      grid(system);
    }

    system.update(GetFrameTime());

    BeginDrawing();

    ClearBackground(BLACK);
    system.draw(Draw::Color{255, 255, 255, 255});

    DrawText(std::to_string(GetFPS()).c_str(), 5, 5, 30, GREEN);

    EndDrawing();

    lastMouse = mouse;
  }
}

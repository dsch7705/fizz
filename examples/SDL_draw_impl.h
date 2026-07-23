#pragma once

#include "fizz/Draw.h"
#include "fizz/Vec2.h"

#include "SDL3/SDL.h"

#include <cmath>
#include <iostream>

extern SDL_Window* sdl_window;
extern SDL_Renderer* sdl_renderer;

inline SDL_Texture* const circle_tex()
{
  static SDL_Texture* tex = nullptr;
  constexpr int tex_w = 128;

  if (!tex) {
    tex = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, tex_w, tex_w);
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

    SDL_SetRenderTarget(sdl_renderer, tex);
    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 0);
    SDL_RenderClear(sdl_renderer);

    int n_verts = 32;
    float radius = tex_w / 2.f;

    constexpr SDL_FColor white{1.f, 1.f, 1.f, 1.f};

    std::vector<SDL_Vertex> verts;
    std::vector<int> inds;

    verts.push_back({{radius, radius}, white});

    for (int i = 0; i < n_verts; ++i) {
      SDL_Vertex v;
      float theta = (6.28318531f / n_verts) * i;
      v.position.x = radius + std::cos(theta) * radius;
      v.position.y = radius + std::sin(theta) * radius;
      v.color = white;
      v.tex_coord = {0, 0};

      verts.push_back(v);

      int current_perimeter_idx = i + 1;
      int next_perimeter_idx = ((i + 1) % n_verts) + 1;

      inds.push_back(0);
      inds.push_back(current_perimeter_idx);
      inds.push_back(next_perimeter_idx);
    }

    SDL_RenderGeometry(sdl_renderer, nullptr, verts.data(), verts.size(), inds.data(), inds.size());
    SDL_SetRenderTarget(sdl_renderer, nullptr);
  }

  return tex;
}

inline void sdl_circle(const Vec2& center, float radius, Draw::Color color)
{
  SDL_Texture* tex = circle_tex();
  SDL_SetTextureColorMod(tex, color.r, color.g, color.b);

  float diam = 2 * radius;
  SDL_FRect dst{.x = center.x - radius, .y = center.y - radius, .w = diam, .h = diam};
  SDL_RenderTexture(sdl_renderer, tex, nullptr, &dst);
}

inline void sdl_line(const Vec2& p0, const Vec2& p1, Draw::Color color)
{
  SDL_SetRenderDrawColor(sdl_renderer, color.r, color.g, color.b, color.a);
  SDL_RenderLine(sdl_renderer, p0.x, p0.y, p1.x, p1.y);
}

inline float calc_dt()
{
  static uint64_t lastPerfCounter = SDL_GetPerformanceCounter();
  static const uint64_t perfFreq = SDL_GetPerformanceFrequency();

  uint64_t perfCounter = SDL_GetPerformanceCounter();
  float dt = (perfCounter - lastPerfCounter) / static_cast<double>(perfFreq);
  lastPerfCounter = perfCounter;

  return dt;
}

inline bool sdl_setup(int w, int h)
{
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cout << "Failed to initialize SDL\n";
    return false;
  }
  SDL_CreateWindowAndRenderer("Cloth", w, h, 0, &sdl_window, &sdl_renderer);
  SDL_SetRenderVSync(sdl_renderer, 1);

  Draw::setCircleCallback(sdl_circle);
  Draw::setLineCallback(sdl_line);

  return true;
}

inline void sdl_cleanup()
{
  SDL_DestroyRenderer(sdl_renderer);
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}
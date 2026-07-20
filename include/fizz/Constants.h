#pragma once

#include <cstdint>

constexpr unsigned int kTargetFPS{0};
constexpr float kTargetFrametime{kTargetFPS ? 1.0 / static_cast<float>(kTargetFPS) : 0};
constexpr float kPhysicStep{0.001};

constexpr float kPi{3.141592653589793};

using ID = uint64_t;
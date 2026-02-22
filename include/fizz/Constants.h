#pragma once

constexpr unsigned int kTargetFPS{60};
constexpr double kTargetFrametime{kTargetFPS ? 1.0 / static_cast<double>(kTargetFPS) : 0};
constexpr double kPhysicStep{0.001};

constexpr double kPi{3.141592653589793};
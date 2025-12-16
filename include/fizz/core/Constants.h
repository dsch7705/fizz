#pragma once

constexpr double kPixelsPerMeter{60.0};
constexpr double kMetersWidth{15.0};
constexpr double kMetersHeight{15.0};
constexpr double kScreenWidth{kMetersWidth * kPixelsPerMeter};
constexpr double kScreenHeight{kMetersHeight * kPixelsPerMeter};

constexpr double kGravity{50.0};

constexpr unsigned int kTargetFPS{60};
constexpr double kTargetFrametime{kTargetFPS ? 1.0 / static_cast<double>(kTargetFPS) : 0};
constexpr double kPhysicStep{0.00001};

constexpr double kPi{3.141592653589793};
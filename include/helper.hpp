#ifndef HELPER_HPP
#define HELPER_HPP

#include <algorithm>

#include "raylib.h"
#include "raylib-cpp.hpp"

inline Color colLighter(Color color, int amount) {
    return Color{
        (unsigned char) std::clamp(color.r + (int)amount, 0, 255),
        (unsigned char) std::clamp(color.g + (int)(amount*1.3), 0, 255),
        (unsigned char) std::clamp(color.b + (int)(amount*1.5), 0, 255),
        255
    };
}
inline double easeInOutCubic(double x) {
    return x < 0.5 ? 4 * x * x * x : 1 - std::pow(-2 * x + 2, 3) / 2;
}
inline float step(float x, float freezeRatio, int level) {
    x *= level;
    int curlevel = (int)(x);
    x -= curlevel;
    if(x >= 1 - freezeRatio) return (float)(curlevel + 1);
    return curlevel + easeInOutCubic(x/(1 - freezeRatio + 1e-15));
}

#endif
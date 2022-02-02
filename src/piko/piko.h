#pragma once

#include <ek/math.h>
#include <ek/time.h>

namespace ek::piko {

static color_t palette[] = {
        RGB(0x000000), //black
        RGB(0x1D2B53), // dark-blue
        RGB(0x7E2553), // dark-purple
        RGB(0x008751), // dark-green
        RGB(0xAB5236), // brown
        RGB(0x5F574F), // dark-gray
        RGB(0xC2C3C7), // light-gray
        RGB(0xFFF1E8), // white
        RGB(0xFF004D), // red
        RGB(0xFFA300), // orange
        RGB(0xFFEC27), // yellow
        RGB(0x00E436), // green
        RGB(0x29ADFF), // blue
        RGB(0x83769C), // indigo
        RGB(0xFF77A8), // pink
        RGB(0xFFCCAA), // peach
//			0x000000, // reset cycle
};

inline static color_t colorf(float index) {
    if (index < 0.0f) index = 0.0f;
    int i = (int) index;
    int e = i + 1;
    auto* colors = palette;
//    int count = sizeof(kPalette) / sizeof(ARGB32);
    int count = 16;
    if (e >= count) e = count - 1;
    return lerp_color(colors[i], colors[e], index - i);
}

inline static float time() {
    return static_cast<float>(ek_time_now());
}

inline static float mid(float x, float y, float z = 0.0f) {
    if (x > y) {
        const float tmp = x;
        x = y;
        y = tmp;
    }
    const float yz = MIN(y, z);
    return MAX(x, yz);
}

inline static float sqr(float a) {
    return a * a;
}

inline static int sgn(float a) {
    return a >= 0.0f ? 1 : -1;
}

inline static float sinu(float unit) {
    return -sinf(unit * MATH_TAU);
}

inline static float cosu(float unit) {
    return cosf(unit * MATH_TAU);
}

}
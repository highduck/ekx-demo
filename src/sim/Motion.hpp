#pragma once

#include <ecx/ecx.hpp>
#include <ek/math.h>

namespace ek::sim {

struct motion_t {
    vec2_t velocity;
};

struct attractor_t {
    float radius = 100.0f;
    float force = 10'000.0f;
};

void update_motion_system(float dt);

}



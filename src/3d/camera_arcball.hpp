#pragma once

#include <ek/math.h>

namespace ek {

struct CameraArcBall {
    vec3_t center = {};
    float distance = 200.0f;
};

void updateCameraArcBall(float dt);

}


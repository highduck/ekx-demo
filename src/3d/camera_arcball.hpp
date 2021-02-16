#pragma once

#include <ek/math/vec.hpp>

namespace ek {

struct CameraArcBall {
    float3 center{};
    float distance = 200.0f;
};

void updateCameraArcBall(float dt);

}


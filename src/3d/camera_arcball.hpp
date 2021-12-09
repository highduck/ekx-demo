#pragma once

#include <ek/math/Vec.hpp>

namespace ek {

struct CameraArcBall {
    Vec3f center{};
    float distance = 200.0f;
};

void updateCameraArcBall(float dt);

}


#pragma once

#include <ek/math.h>
#include <ecx/ecx.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    vec3_t center;
    float distance;
} camera_arc_ball_t;

extern ECX_DEFINE_TYPE(camera_arc_ball_t);

void update_camera_arc_ball(float dt);

#ifdef __cplusplus
}
#endif


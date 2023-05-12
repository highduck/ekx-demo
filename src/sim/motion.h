#pragma once

#include <ek/math.h>
#include <ecx/ecx.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    vec2_t velocity;
} motion_t;

typedef struct {
    float radius;
    float force;
} attractor_t;

extern ECX_DEFINE_TYPE(motion_t);
extern ECX_DEFINE_TYPE(attractor_t);

void update_motion_system(float dt);

#ifdef __cplusplus
}
#endif

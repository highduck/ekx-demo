#ifndef SIM_FOLLOW_H
#define SIM_FOLLOW_H

#include <ecx/ecx.h>
#include <ek/math.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    FOLLOW_INTEGRATION_EXP = 0,
    FOLLOW_INTEGRATION_STEPS = 1,
    FOLLOW_INTEGRATION_NONE = 2,
} follow_integration_mode;

typedef struct {

    vec2_t offset;
    vec2_t target;
    entity_t target_entity;

    float k;
    float fixed_frame_rate;

    int n;
    int counter;
    float time_accum;

    int frame;
    int reset_in;

    follow_integration_mode integration;

} follow_target_t;

extern ecx_component_type FollowMouse;
extern ecx_component_type FollowTarget;

void Follow_setup(void);
void Follow_update(float dt);

#define add_follow_target(e) ((follow_target_t*)add_component(&FollowTarget, e))
#define add_follow_mouse(e) ((void*)add_component(&FollowMouse, e))

#ifdef __cplusplus
}
#endif

#endif // SIM_FOLLOW_H

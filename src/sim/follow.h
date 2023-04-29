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

typedef void follow_mouse_t;

extern ECX_DEFINE_TYPE(follow_mouse_t);
extern ECX_DEFINE_TYPE(follow_target_t);

void setup_follow(void);

void update_follow(void);

#define add_follow_target(e) ECX_ADD(follow_target_t,e)
#define add_follow_mouse(e) ECX_ADD(follow_mouse_t,e)

#ifdef __cplusplus
}
#endif

#endif // SIM_FOLLOW_H

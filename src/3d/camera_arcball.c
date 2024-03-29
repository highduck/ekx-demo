#include "camera_arcball.h"

#include "../render3d/scene3d.h"
#include <ek/scenex/interaction_system.h>

#include <ekx/app/input_state.h>

ECX_DEFINE_TYPE(camera_arc_ball_t);

void update_camera_arc_ball(float dt) {
    static vec2_t prev_pointer = {};
    static bool prev_down = false;

    vec2_t delta = {};
    if (g_interaction_system.pointerDown_ && !prev_down) {
        prev_down = true;
        prev_pointer = g_interaction_system.pointerScreenPosition_;
    }
    if (!g_interaction_system.pointerDown_ && prev_down) {
        prev_down = false;
    }
    if (prev_down) {
        vec2_t cur = g_interaction_system.pointerScreenPosition_;
        delta = sub_vec2(cur, prev_pointer);
        prev_pointer = cur;
    }
    ECX_FOR_DRAFT(camera_arc_ball_t) {
        entity_t e = get_entity(&ECX_ID(camera_arc_ball_t), h_camera_arc_ball_t);
        camera_arc_ball_t* arc_ball = (camera_arc_ball_t*)get_component_data(&ECX_ID(camera_arc_ball_t), h_camera_arc_ball_t, 0);
        camera3d_t* camera_data = get_camera3d(e);
        transform3d_t* camera_transform = get_transform3d(e);
        if(camera_data && camera_transform) {
            vec3_t dir = normalize_vec3(sub_vec3(arc_ball->center, camera_transform->position));
            vec3_t r = cross_vec3(dir, camera_data->up);
            vec3_t t = cross_vec3(r, dir);
            if (length_vec3(r) < 0.001f) {
                t = cross_vec3(dir, vec3(0, 1, 0));
                r = neg_vec3(cross_vec3(t, dir));
            }

            vec3_t translation = add_vec3(scale_vec3(r, -delta.x), scale_vec3(t, delta.y));
            camera_transform->position = add_vec3(camera_transform->position, translation);

            float dd = 0.0f;
            if (is_key(EK_KEYCODE_W)) {
                dd -= 10.0f * dt;
            }
            if (is_key(EK_KEYCODE_S)) {
                dd += 10.0f * dt;
            }
            arc_ball->distance = fmaxf(arc_ball->distance + dd, 0.0f);

            dir = normalize_vec3(sub_vec3(arc_ball->center, camera_transform->position));
            camera_transform->position = sub_vec3(arc_ball->center, scale_vec3(dir, arc_ball->distance));
            camera_transform->rotation = quat_to_euler_angles(quat_look_at_rh(dir, camera_data->up));
        }
    }
}


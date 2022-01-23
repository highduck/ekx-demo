#include "camera_arcball.hpp"

#include <ecxx/ecxx.hpp>
#include <ek/scenex/3d/Camera3D.hpp>
#include <ek/scenex/3d/Transform3D.hpp>
#include <ek/scenex/InteractionSystem.hpp>
#include <ek/util/ServiceLocator.hpp>
#include <ek/scenex/app/input_controller.hpp>

namespace ek {

void updateCameraArcBall(float dt) {

    const auto* im = g_interaction_system;
    static vec2_t prev_pointer = {};
    static bool prev_down = false;

    vec2_t delta = {};
    if (im->pointerDown_ && !prev_down) {
        prev_down = true;
        prev_pointer = im->pointerScreenPosition_;
    }
    if (!im->pointerDown_ && prev_down) {
        prev_down = false;
    }
    if (prev_down) {
        vec2_t cur = im->pointerScreenPosition_;
        delta = cur - prev_pointer;
        prev_pointer = cur;
    }
    for (auto e: ecs::view<Camera3D, CameraArcBall, Transform3D>()) {
        auto& arc_ball = e.get<CameraArcBall>();
        auto& camera_data = e.get<Camera3D>();
        auto& camera_transform = e.get<Transform3D>();

        auto dir = normalize_vec3(sub_vec3(arc_ball.center, camera_transform.position));
        auto r = cross_vec3(dir, camera_data.up);
        auto t = cross_vec3(r, dir);
        if (length_vec3(r) < 0.001f) {
            t = cross_vec3(dir, vec3(0, 1, 0));
            r = neg_vec3(cross_vec3(t, dir));
        }

        vec3_t translation = add_vec3(scale_vec3(r, -delta.x), scale_vec3(t, delta.y));
        camera_transform.position = add_vec3(camera_transform.position, translation);

        auto dd = 0.0f;
        auto* input = g_input_controller;
        if (input->is_key(EK_KEYCODE_W)) {
            dd -= 10.0f * dt;
        }
        if (input->is_key(EK_KEYCODE_S)) {
            dd += 10.0f * dt;
        }
        arc_ball.distance = fmaxf(arc_ball.distance + dd, 0.0f);

        dir = normalize_vec3(sub_vec3(arc_ball.center, camera_transform.position));
        camera_transform.position = sub_vec3(arc_ball.center, scale_vec3(dir, arc_ball.distance));
        camera_transform.rotation = quat_to_euler_angles(quat_look_at_rh(dir,camera_data.up));
    }
}

}
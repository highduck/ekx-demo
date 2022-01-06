#include "camera_arcball.hpp"

#include <ecxx/ecxx.hpp>
#include <ek/scenex/3d/Camera3D.hpp>
#include <ek/scenex/3d/Transform3D.hpp>
#include <ek/scenex/InteractionSystem.hpp>
#include <ek/util/ServiceLocator.hpp>
#include <ek/scenex/app/input_controller.hpp>
#include <ek/math/Quaternion.hpp>

namespace ek {

void updateCameraArcBall(float dt) {

    const auto& im = Locator::ref<InteractionSystem>();
    static vec2_t prev_pointer = {};
    static bool prev_down = false;

    vec2_t delta = {};
    if (im.pointerDown_ && !prev_down) {
        prev_down = true;
        prev_pointer = im.pointerScreenPosition_;
    }
    if (!im.pointerDown_ && prev_down) {
        prev_down = false;
    }
    if (prev_down) {
        vec2_t cur = im.pointerScreenPosition_;
        delta = cur - prev_pointer;
        prev_pointer = cur;
    }
    for (auto e: ecs::view<Camera3D, CameraArcBall, Transform3D>()) {
        auto& arc_ball = e.get<CameraArcBall>();
        auto& camera_data = e.get<Camera3D>();
        auto& camera_transform = e.get<Transform3D>();

        auto dir = vec3_normalize(vec3_sub(arc_ball.center, camera_transform.position));
        auto r = vec3_cross(dir, camera_data.up);
        auto t = vec3_cross(r, dir);
        if (vec3_length(r) < 0.001f) {
            t = vec3_cross(dir, vec3(0, 1, 0));
            r = vec3_neg(vec3_cross(t, dir));
        }

        vec3_t translation = vec3_add(vec3_scale(r, -delta.x), vec3_scale(t, delta.y));
        camera_transform.position = vec3_add(camera_transform.position, translation);

        auto dd = 0.0f;
        auto& input = Locator::ref<input_controller>();
        if (input.is_key(EK_KEYCODE_W)) {
            dd -= 10.0f * dt;
        }
        if (input.is_key(EK_KEYCODE_S)) {
            dd += 10.0f * dt;
        }
        arc_ball.distance = fmaxf(arc_ball.distance + dd, 0.0f);

        dir = vec3_normalize(vec3_sub(arc_ball.center, camera_transform.position));
        camera_transform.position = vec3_sub(arc_ball.center, vec3_scale(dir, arc_ball.distance));
        camera_transform.rotation = euler_angles(quat_look_at_rh((Vec3f)dir, (Vec3f)camera_data.up));
    }
}

}
#include "Motion.hpp"

#include <ek/scenex/2d/transform2d.h>

namespace ek::sim {

inline const float WIDTH = 360;
inline const float HEIGHT = 480;

struct AttractorsState {
    attractor_t props;
    vec2_t position;
};

void update_motion_system(float dt) {
    AttractorsState attractors[10];
    uint32_t attractors_num = 0;

    for (entity_t e : ecs::view<attractor_t>()) {
        attractors[attractors_num++] = AttractorsState{
           *ecs::get<attractor_t>(e),
           get_transform2d(e)->pos
        };
    }

    const auto dumpFactor = expf(-6.0f * dt);
    const aabb2_t bounds = aabb2_from_rect(rect_wh(WIDTH, HEIGHT));
    for (entity_t e : ecs::view<motion_t>()) {
        motion_t* mot = ecs::get<motion_t>(e);
        transform2d_t* tra = get_transform2d(e);

        vec2_t p = tra->pos;
        vec2_t v = mot->velocity;

        for (uint32_t i = 0; i < attractors_num; ++i) {
            const AttractorsState attractor = attractors[i];
            const vec2_t diff = attractor.position - p;
            const float len = length_vec2(diff);
            const float factor = 1.0f - saturate(len / attractor.props.radius);
            v += (dt * attractor.props.force * factor * factor / len) * diff;
        }

        p += dt * mot->velocity;

        if (p.x < bounds.x0) {
            v.x = -v.x;
            p.x = bounds.x0;
        } else if (p.x > bounds.x1) {
            v.x = -v.x;
            p.x = bounds.x1;
        }
        if (p.y < bounds.y0) {
            v.y = -v.y;
            p.y = bounds.y0;
        } else if (p.y > bounds.y1) {
            v.y = -v.y;
            p.y = bounds.y1;
        }

        mot->velocity = v * dumpFactor;
        tra->pos = p;
    }
}

}
#include "Motion.hpp"

#include <ek/scenex/2d/Transform2D.hpp>
#include <ek/ds/Array.hpp>

namespace ek::sim {

inline const float WIDTH = 360;
inline const float HEIGHT = 480;

struct AttractorsState {
    attractor_t props;
    vec2_t position;
};

void update_motion_system(float dt) {
    FixedArray<AttractorsState, 10> attractors;
    for (auto e_ : ecs::view<attractor_t>()) {
        const auto e = e_.index;
        attractors.push_back(AttractorsState{
           ecs::get<attractor_t>(e),
           ecs::get<Transform2D>(e).getPosition()
        });
    }
    const auto sz = attractors.size();
    const auto* attrs = attractors.data();

    const auto dumpFactor = expf(-6.0f * dt);
    const aabb2_t bounds = aabb2_from_rect(rect_wh(WIDTH, HEIGHT));
    for (auto e_ : ecs::view<motion_t>()) {
        auto e = e_.index;
        auto& mot = ecs::get<motion_t>(e);
        auto& tra = ecs::get<Transform2D>(e);

        auto p = tra.getPosition();
        auto v = mot.velocity;

        for (unsigned i = 0; i < sz; ++i) {
            const auto attractor = attrs[i];
            const auto diff = attractor.position - p;
            const auto len = length_vec2(diff);
            const float factor = 1.0f - saturate(len / attractor.props.radius);
            v += (dt * attractor.props.force * factor * factor / len) * diff;
        }

        p += dt * mot.velocity;

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

        mot.velocity = v * dumpFactor;
        tra.setPosition(p);
    }
}

}
#include "Motion.hpp"

#include <ek/scenex/2d/Transform2D.hpp>
#include <ek/ds/Array.hpp>

namespace ek::sim {

inline const float WIDTH = 360;
inline const float HEIGHT = 480;

struct AttractorsState {
    attractor_t props;
    Vec2f position;
};

void update_motion_system(float dt) {
    FixedArray<AttractorsState, 10> attractors;
    const auto& w = ecs::the_world;
    for (auto e_ : ecs::view<attractor_t>()) {
        const auto e = e_.index;
        attractors.push_back(AttractorsState{
            w.get<attractor_t>(e),
            w.get<Transform2D>(e).getPosition()
        });
    }
    const auto sz = attractors.size();
    const auto* attrs = attractors.data();

    const auto dumpFactor = expf(-6.0f * dt);
    const Rect2f bounds{0.0f, 0.0f, WIDTH, HEIGHT};
    for (auto e_ : ecs::view<motion_t>()) {
        auto e = e_.index;
        auto& mot = w.get<motion_t>(e);
        auto& tra = w.get<Transform2D>(e);

        auto p = tra.getPosition();
        auto v = mot.velocity;


        for (unsigned i = 0; i < sz; ++i) {
            const auto attractor = attrs[i];
            const auto diff = attractor.position - p;
            const auto len = length(diff);
            const float factor = 1.0f - Math::clamp(len / attractor.props.radius);
            v += dt * attractor.props.force * factor * factor * diff * (1.0f / len);
        }

        p += dt * mot.velocity;

        if (p.x < bounds.x) {
            v.x = -v.x;
            p.x = bounds.x;
        } else if (p.x > bounds.right()) {
            v.x = -v.x;
            p.x = bounds.right();
        }
        if (p.y < bounds.y) {
            v.y = -v.y;
            p.y = bounds.y;
        } else if (p.y > bounds.bottom()) {
            v.y = -v.y;
            p.y = bounds.bottom();
        }

        mot.velocity = v * dumpFactor;
        tra.setPosition(p);
    }
}

}
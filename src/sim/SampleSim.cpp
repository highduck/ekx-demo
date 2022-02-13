#include "SampleSim.hpp"

#include <ek/time.h>
#include <ek/rnd.h>
#include "Motion.hpp"
#include <ek/scenex/SceneFactory.hpp>
#include <ek/scenex/2d/Transform2D.hpp>
#include <ek/scenex/base/Node.hpp>
#include <ek/scenex/2d/Display2D.hpp>
#include <ek/goodies/helpers/Trail2D.hpp>
#include <ek/goodies/helpers/follow.h>
#include <ui/minimal.hpp>
#include <ek/scenex/base/DestroyTimer.hpp>

namespace ek {

namespace sim {

inline const float WIDTH = 360;
inline const float HEIGHT = 480;

unsigned ParticlesAddCount = 5000;
unsigned ParticlesMaxCount = 25000;

void add_objects(ecs::Entity game, unsigned addCount) {
    for (unsigned i = 0; i < addCount; ++i) {
        auto q = createNode2D();
        set_touchable(q, false);
        const vec2_t pos = vec2(random_range_f(0.0f, WIDTH),
                                random_range_f(0.0f, HEIGHT));
        ecs::get<Transform2D>(q).set_position(pos);

        auto& mot = ecs::add<motion_t>(q);
        mot.velocity = vec2(random_range_f(-50.0f, 50.0f), random_range_f(-50.0f, 50.0f));

        auto* quad = quad2d_setup(q);
        quad->rect = rect(-0.25f, -0.25f, 0.5f, 0.5f);
        quad->setColor(RGB(0xFFFFFF));

        append(game, q);

        auto& trail_data = ecs::add<Trail2D>(q);
        trail_data.drain_speed = 1.0f;

        auto trailRenderer = createNode2D();

        auto& renderer = ecs::add<TrailRenderer2D>(trailRenderer);
        renderer.target = q;
        renderer.width = 0.25f;
        renderer.minWidth = 0.0f;
        ecs::add<Display2D>(trailRenderer).draw = trail_renderer2d_draw;
        append(game, trailRenderer);
    }
}

}

SampleSim::SampleSim() {

    using namespace ek::sim;

    title = "SIMULATION";

    particlesContainer = createNode2D(H("particles"));

    append(container, particlesContainer);

    auto mouse_entity = createNode2D(H("Mouse"));
    ecs::add<mouse_follow_comp>(mouse_entity);

    auto attractor_entity = createNode2D(H("Follower"));
    ecs::add<attractor_t>(attractor_entity);
    auto& attr = ecs::add<target_follow_comp>(attractor_entity);
    attr.target_entity = mouse_entity;
    attr.k = 0.1f;
    ecs::get<attractor_t>(attractor_entity).radius = 100.0f;
    ecs::get<attractor_t>(attractor_entity).force = 5'000.0f;

    append(container, mouse_entity);
    append(container, attractor_entity);

    attractor_entity = createNode2D(H("Centroid"));
    ecs::add<attractor_t>(attractor_entity);
    ecs::get<attractor_t>(attractor_entity).radius = 200.0f;
    ecs::get<attractor_t>(attractor_entity).force = -1000.0f;
    ecs::get<Transform2D>(attractor_entity).set_position(300.0f, 400.0f);
    append(container, attractor_entity);

    countLabel = createNode2D(H("lbl"));
    addText(countLabel, "");
    ecs::get<Transform2D>(countLabel).set_position(360.0f / 2.0f, 15.0f);
    append(container, countLabel);

    add_objects(particlesContainer, ParticlesAddCount);
    particlesCount += ParticlesAddCount;
    updateCountLabel();

    auto btn = createButton("ADD 5000", [this] {
        if(particlesCount < ParticlesMaxCount) {
            add_objects(particlesContainer, ParticlesAddCount);
            particlesCount += ParticlesAddCount;

            updateCountLabel();
        }
    });
    ecs::get<Transform2D>(btn).set_position(360.0f / 2.0f, 60.0f);
    ecs::get<Text2D>(btn).rect = {{-100, -25, 200, 50}};
    append(container, btn);

    btn = createButton("RESET", [this] {
        particlesCount = 0;
        destroy_children_later(particlesContainer);

        updateCountLabel();
    });
    ecs::get<Transform2D>(btn).set_position(360.0f / 2.0f, 120.0f);
    ecs::get<Text2D>(btn).rect = {{-100, -25, 200, 50}};
    append(container, btn);
}

void SampleSim::draw() {
    SampleBase::draw();
}

void SampleSim::update(float dt) {
    SampleBase::update(dt);

    update_mouse_follow_comps();
    update_target_follow_comps(dt);
    sim::update_motion_system(g_time_layers[TIME_LAYER_GAME].dt);
}

void SampleSim::updateCountLabel() const {
    set_text_f(countLabel, "%d", particlesCount);
}

}

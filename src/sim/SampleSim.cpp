#include "SampleSim.hpp"

#include <ek/time.h>
#include <ek/rnd.h>
#include "Motion.hpp"
#include <ek/scenex/scene_factory.h>
#include <ek/scenex/2d/Transform2D.hpp>
#include <ek/scenex/base/node.h>
#include <ek/scenex/2d/Display2D.hpp>
#include <ek/goodies/helpers/Trail2D.hpp>
#include <ek/goodies/helpers/follow.h>
#include <ui/minimal.hpp>
#include <ek/scenex/base/destroy_timer.h>

namespace ek {

namespace sim {

inline const float WIDTH = 360;
inline const float HEIGHT = 480;

unsigned ParticlesAddCount = 5000;
unsigned ParticlesMaxCount = 25000;

void add_objects(entity_t game, unsigned addCount) {
    for (unsigned i = 0; i < addCount; ++i) {
        entity_t q = create_node2d(0);
        set_touchable(q, false);
        const vec2_t pos = vec2(random_range_f(0.0f, WIDTH),
                                random_range_f(0.0f, HEIGHT));
        ecs::get<Transform2D>(q).set_position(pos);

        motion_t& mot = ecs::add<motion_t>(q);
        mot.velocity = vec2(random_range_f(-50.0f, 50.0f), random_range_f(-50.0f, 50.0f));

        Quad2D* quad = quad2d_setup(q);
        quad->rect = rect(-0.25f, -0.25f, 0.5f, 0.5f);
        quad->setColor(RGB(0xFFFFFF));

        append(game, q);

        Trail2D& trail_data = ecs::add<Trail2D>(q);
        trail_data.drain_speed = 1.0f;

        entity_t trail_renderer = create_node2d(0);

        TrailRenderer2D& renderer = ecs::add<TrailRenderer2D>(trail_renderer);
        renderer.target = q;
        renderer.width = 0.25f;
        renderer.minWidth = 0.0f;
        ecs::add<Display2D>(trail_renderer).draw = trail_renderer2d_draw;
        append(game, trail_renderer);
    }
}

}

SampleSim::SampleSim() {

    using namespace ek::sim;

    title = "SIMULATION";

    particles_container = create_node2d(H("particles"));

    append(container, particles_container);

    entity_t mouse_entity = create_node2d(H("Mouse"));
    ecs::add<mouse_follow_comp>(mouse_entity);

    entity_t attractor_entity = create_node2d(H("Follower"));
    ecs::add<attractor_t>(attractor_entity);
    target_follow_comp& attr = ecs::add<target_follow_comp>(attractor_entity);
    attr.target_entity = mouse_entity;
    attr.k = 0.1f;
    ecs::get<attractor_t>(attractor_entity).radius = 100.0f;
    ecs::get<attractor_t>(attractor_entity).force = 5'000.0f;

    append(container, mouse_entity);
    append(container, attractor_entity);

    attractor_entity = create_node2d(H("Centroid"));
    ecs::add<attractor_t>(attractor_entity);
    ecs::get<attractor_t>(attractor_entity).radius = 200.0f;
    ecs::get<attractor_t>(attractor_entity).force = -1000.0f;
    ecs::get<Transform2D>(attractor_entity).set_position(300.0f, 400.0f);
    append(container, attractor_entity);

    count_label = create_node2d(H("lbl"));
    addText(count_label, "");
    ecs::get<Transform2D>(count_label).set_position(360.0f / 2.0f, 15.0f);
    append(container, count_label);

    add_objects(particles_container, ParticlesAddCount);
    particles_count += ParticlesAddCount;
    updateCountLabel();

    entity_t btn = createButton("ADD 5000", [this] {
        if(particles_count < ParticlesMaxCount) {
            add_objects(particles_container, ParticlesAddCount);
            particles_count += ParticlesAddCount;

            updateCountLabel();
        }
    });
    ecs::get<Transform2D>(btn).set_position(360.0f / 2.0f, 60.0f);
    ecs::get<Text2D>(btn).rect = {{-100, -25, 200, 50}};
    append(container, btn);

    btn = createButton("RESET", [this] {
        particles_count = 0;
        destroy_children_later(particles_container, 0, 0);

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
    set_text_f(count_label, "%d", particles_count);
}

}

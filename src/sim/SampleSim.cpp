#include "SampleSim.hpp"

#include "Motion.hpp"
#include <ek/scenex/2d/display2d.h>
#include <ek/rnd.h>
#include <ek/scenex/scene_factory.h>
#include <ek/scenex/2d/transform2d.h>
#include <ek/scenex/base/node.h>
#include <ek/scenex/2d/text2d.h>
#include <ek/goodies/trail2d.h>
#include <sim/follow.h>
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
        get_transform2d(q)->pos = vec2(random_range_f(0.0f, WIDTH),
                                       random_range_f(0.0f, HEIGHT));;

        motion_t* mot = ecs::add<motion_t>(q);
        mot->velocity = vec2(random_range_f(-50.0f, 50.0f), random_range_f(-50.0f, 50.0f));

        set_color_quad(q, rect(-0.25f, -0.25f, 0.5f, 0.5f), RGB(0xFFFFFF));
        append(game, q);

        trail2d_t* trail_data = add_trail2d(q);
        trail_data->drain_speed = 1.0f;

        entity_t trail_renderer = create_node2d(0);

        trail_renderer2d_t * renderer = add_trail_renderer2d(trail_renderer, q);
        renderer->width = 0.25f;
        renderer->minWidth = 0.0f;
        append(game, trail_renderer);
    }
}

}

SampleSim::SampleSim() {

    using namespace ek::sim;

    title = "SIMULATION";

    particles_container = create_node2d(H("particles"));

    append(container, particles_container);

    {
        entity_t mouse_entity = create_node2d(H("Mouse"));
        add_follow_mouse(mouse_entity);

        entity_t attractor_entity = create_node2d(H("Follower"));
        attractor_t* attractor = ecs::add<attractor_t>(attractor_entity);
        follow_target_t* attr = add_follow_target(attractor_entity);
        attr->target_entity = mouse_entity;
        attr->k = 0.1f;
        attractor->radius = 100.0f;
        attractor->force = 5000.0f;

        append(container, mouse_entity);
        append(container, attractor_entity);
    }

    {
        entity_t attractor_entity = create_node2d(H("Centroid"));
        attractor_t* attractor = ecs::add<attractor_t>(attractor_entity);
        attractor->radius = 200.0f;
        attractor->force = -1000.0f;
        get_transform2d(attractor_entity)->pos = vec2(300.0f, 400.0f);
        append(container, attractor_entity);
    }

    count_label = create_node2d(H("lbl"));
    addText(count_label, "");
    get_transform2d(count_label)->pos = vec2(360.0f / 2.0f, 15.0f);
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
    get_transform2d(btn)->pos = vec2(360.0f / 2.0f, 60.0f);
    get_text2d(btn)->rect = {{-100, -25, 200, 50}};
    append(container, btn);

    btn = createButton("RESET", [this] {
        particles_count = 0;
        destroy_children_later(particles_container, 0, 0);

        updateCountLabel();
    });
    get_transform2d(btn)->pos = vec2(360.0f / 2.0f, 120.0f);
    get_text2d(btn)->rect = {{-100, -25, 200, 50}};
    append(container, btn);
}

void SampleSim::draw() {
    SampleBase::draw();
}

void SampleSim::update(float dt) {
    SampleBase::update(dt);
    Follow_update(dt);
    sim::update_motion_system(g_time_layers[TIME_LAYER_GAME].dt);
}

void SampleSim::updateCountLabel() const {
    set_text_f(count_label, "%d", particles_count);
}

}

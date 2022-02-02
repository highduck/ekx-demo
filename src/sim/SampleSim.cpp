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

void add_objects(ecs::EntityApi game, unsigned addCount) {
    for (unsigned i = 0; i < addCount; ++i) {
        auto q = createNode2D();
        q.get<Node>().setTouchable(false);
        const vec2_t pos = vec2(random_range_f(0.0f, WIDTH),
                                random_range_f(0.0f, HEIGHT));
        q.get<Transform2D>().setPosition(pos);

        auto& mot = q.assign<motion_t>();
        mot.velocity = vec2(random_range_f(-50.0f, 50.0f), random_range_f(-50.0f, 50.0f));

        auto* quad = quad2d_setup(q.index);
        quad->rect = rect(-0.25f, -0.25f, 0.5f, 0.5f);
        quad->setColor(RGB(0xFFFFFF));

        append(game, q);

        auto& trail_data = q.assign<Trail2D>();
        trail_data.drain_speed = 1.0f;

        auto trailRenderer = createNode2D();

        auto& renderer = trailRenderer.assign<TrailRenderer2D>();
        renderer.target = q;
        renderer.width = 0.25f;
        renderer.minWidth = 0.0f;
        trailRenderer.assign<Display2D>().draw = trail_renderer2d_draw;
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
    mouse_entity.assign<mouse_follow_comp>();

    auto attractor_entity = createNode2D(H("Follower"));
    attractor_entity.assign<attractor_t>();
    auto& attr = attractor_entity.assign<target_follow_comp>();
    attr.target_entity = ecs::EntityRef{mouse_entity};
    attr.k = 0.1f;
    attractor_entity.get<attractor_t>().radius = 100.0f;
    attractor_entity.get<attractor_t>().force = 5'000.0f;

    append(container, mouse_entity);
    append(container, attractor_entity);

    attractor_entity = createNode2D(H("Centroid"));
    attractor_entity.assign<attractor_t>();
    attractor_entity.get<attractor_t>().radius = 200.0f;
    attractor_entity.get<attractor_t>().force = -1000.0f;
    attractor_entity.get<Transform2D>().setPosition(300.0f, 400.0f);
    append(container, attractor_entity);

    countLabel = createNode2D(H("lbl"));
    addText(countLabel, "");
    countLabel.get<Transform2D>().setPosition(360.0f / 2.0f, 15.0f);
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
    btn.get<Transform2D>().setPosition(360.0f / 2.0f, 60.0f);
    btn.get<Text2D>().rect = {{-100, -25, 200, 50}};
    append(container, btn);

    btn = createButton("RESET", [this] {
        particlesCount = 0;
        destroyChildrenDelay(particlesContainer);

        updateCountLabel();
    });
    btn.get<Transform2D>().setPosition(360.0f / 2.0f, 120.0f);
    btn.get<Text2D>().rect = {{-100, -25, 200, 50}};
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
    countLabel.get<Text2D>().text = String::format("%d", particlesCount);
}

}

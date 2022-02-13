#include "sample_piko.hpp"

#include "examples.h"
#include <ek/scenex/base/Node.hpp>
#include <ek/scenex/2d/Transform2D.hpp>
#include <ek/scenex/2d/Display2D.hpp>
#include <ek/scenex/SceneFactory.hpp>

namespace ek {

namespace piko {

void create(ecs::Entity container) {

    ecs::Entity e;
    e = createNode2D(H("book"));
    ecs::add<Display2D>(e).callback = draw_book;
    //ecs::get<transform_2d>(e).scale = {2.0f, 2.0f};
    ecs::get<Transform2D>(e).set_position(20.0f, 20.0f);
    // TODO: fix scissors stats
    {
        auto& bounds = ecs::add<Bounds2D>(e);
        bounds.rect = rect_wh(128, 128);
        bounds.flags |= BOUNDS_2D_SCISSORS;
    }
    append(container, e);

    e = createNode2D(H("dna"));
    ecs::add<Display2D>(e).callback = draw_dna;
//    ecs::get<transform_2d>(e).scale = {2.0f, 2.0f};
    ecs::get<Transform2D>(e).set_position(20.0f, 20.0f + 128.0f + 10.0f);
    // TODO: fix scissors stats
    {
        auto& bounds = ecs::add<Bounds2D>(e);
        bounds.rect = rect_wh(128, 128);
        bounds.flags |= BOUNDS_2D_SCISSORS;
    }
    append(container, e);

    e = createNode2D(H("diamonds"));
    ecs::add<diamonds>(e);
    ecs::add<Display2D>(e).callback = draw_diamonds;
    ecs::get<Transform2D>(e).set_position(20.0f + 128.0f + 10.0f, 20.0f);
    {
        auto& bounds = ecs::add<Bounds2D>(e);
        bounds.rect = rect_wh(128, 128);
        bounds.flags |= BOUNDS_2D_SCISSORS;
    }
    append(container, e);
}

}

SamplePiko::SamplePiko() {
    piko::create(container);
    title = "PIKO PIKO";
}

void SamplePiko::draw() {
    SampleBase::draw();
}

void SamplePiko::update(float dt) {
    SampleBase::update(dt);
}

}
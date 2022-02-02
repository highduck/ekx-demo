#include "sample_piko.hpp"

#include "examples.h"
#include <ek/scenex/base/Node.hpp>
#include <ek/scenex/2d/Transform2D.hpp>
#include <ek/scenex/2d/Display2D.hpp>
#include <ek/scenex/SceneFactory.hpp>

namespace ek {

namespace piko {

void create(ecs::EntityApi container) {

    ecs::EntityApi e;
    e = createNode2D(H("book"));
    e.assign<Display2D>().callback = draw_book;
    //ecs::get<transform_2d>(e).scale = {2.0f, 2.0f};
    e.get<Transform2D>().setPosition(20.0f, 20.0f);
    // TODO: fix scissors stats
    {
        auto& bounds = e.assign<Bounds2D>();
        bounds.rect = rect_wh(128, 128);
        bounds.flags |= BOUNDS_2D_SCISSORS;
    }
    append(container, e);

    e = createNode2D(H("dna"));
    e.assign<Display2D>().callback = draw_dna;
//    ecs::get<transform_2d>(e).scale = {2.0f, 2.0f};
    e.get<Transform2D>().setPosition(20.0f, 20.0f + 128.0f + 10.0f);
    // TODO: fix scissors stats
    {
        auto& bounds = e.assign<Bounds2D>();
        bounds.rect = rect_wh(128, 128);
        bounds.flags |= BOUNDS_2D_SCISSORS;
    }
    append(container, e);

    e = createNode2D(H("diamonds"));
    e.assign<diamonds>();
    e.assign<Display2D>().callback = draw_diamonds;
    e.get<Transform2D>().setPosition(20.0f + 128.0f + 10.0f, 20.0f);
    {
        auto& bounds = e.assign<Bounds2D>();
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
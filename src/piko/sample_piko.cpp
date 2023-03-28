#include "sample_piko.hpp"

#include "examples.h"
#include <ek/scenex/base/node.h>
#include <ek/scenex/2d/Transform2D.hpp>
#include <ek/scenex/2d/Display2D.hpp>
#include <ek/scenex/scene_factory.h>
#include "../scripting/scripting.h"
#include "ui/minimal.hpp"

namespace ek {

namespace piko {

void create(entity_t container) {
    entity_t e = create_node2d(H("book"));
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

    e = create_node2d(H("dna"));
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

    e = create_node2d(H("diamonds"));
    ecs::add<diamonds>(e);
    ecs::add<Display2D>(e).callback = draw_diamonds;
    ecs::get<Transform2D>(e).set_position(20.0f + 128.0f + 10.0f, 20.0f);
    {
        auto& bounds = ecs::add<Bounds2D>(e);
        bounds.rect = rect_wh(128, 128);
        bounds.flags |= BOUNDS_2D_SCISSORS;
    }
    append(container, e);


    float spaceY = 40.0f;
    rect_t default_rect = rect(-100, -15, 200, 30);
    vec2_t pos = vec2(360.0f / 2, 300.0f);
    auto btn = createButton("RELOAD", [] {
        load_script("assets/scripts/main.js");
    });
    set_position(btn, pos);
    ecs::get<Text2D>(btn).rect = default_rect;
    append(container, btn);
    pos.y += spaceY;
}

}

SamplePiko::SamplePiko() {
    piko::create(container);
    title = "PIKO PIKO";

    js_initialize();
    js_register();
    load_script("assets/scripts/main.js");
}

void SamplePiko::draw() {
    SampleBase::draw();
}

void SamplePiko::update(float dt) {
    SampleBase::update(dt);
}

}
#include "sample_piko.h"

#include "../scripting/scripting.h"
#include "examples.h"
#include "ui/minimal_ui.h"
#include <ek/scenex/2d/display2d.h>
#include <ek/scenex/2d/text2d.h>
#include <ek/scenex/2d/transform2d.h>
#include <ek/scenex/base/node.h>
#include <ek/scenex/scene_factory.h>

static void on_reload_script(const node_event_t* event) {
    UNUSED(event);
    load_script("assets/scripts/main.js");
}

static void piko_create(entity_t container) {
    entity_t e = create_node2d(H("book"));
    add_display2d(e)->draw = draw_book;
    get_transform2d(e)->pos = vec2(20.0f, 20.0f);
    {
        bounds2d_t* bounds = add_bounds2d(e);
        bounds->rect = rect_wh(128, 128);
        bounds->flags |= BOUNDS_2D_SCISSORS;
    }
    append(container, e);

    e = create_node2d(H("dna"));
    add_display2d(e)->draw = draw_dna;
    get_transform2d(e)->pos = vec2(20.0f, 20.0f + 128.0f + 10.0f);
    {
        bounds2d_t* bounds = add_bounds2d(e);
        bounds->rect = rect_wh(128, 128);
        bounds->flags |= BOUNDS_2D_SCISSORS;
    }
    append(container, e);

    e = create_node2d(H("diamonds"));
    add_display2d(e)->draw = draw_diamonds;
    get_transform2d(e)->pos = vec2(20.0f + 128.0f + 10.0f, 20.0f);
    {
        bounds2d_t* bounds = add_bounds2d(e);
        bounds->rect = rect_wh(128, 128);
        bounds->flags |= BOUNDS_2D_SCISSORS;
    }
    append(container, e);


    float spaceY = 40.0f;
    rect_t default_rect = rect(-100, -15, 200, 30);
    vec2_t pos = vec2(360.0f / 2, 300.0f);
    entity_t btn = create_button("RELOAD", on_reload_script);
    set_position(btn, pos);
    get_text2d(btn)->rect = default_rect;
    append(container, btn);
    pos.y += spaceY;
}

static void start_sample_piko(entity_t container) {
    piko_create(container);
    load_script("assets/scripts/main.js");
}

sample_t sample_piko(void) {
    js_initialize();
    js_register();
    sample_t s = INIT_ZERO;
    s.title = "PIKO";
    s.start = start_sample_piko;
    s.stop = stop_diamonds;
    return s;
}

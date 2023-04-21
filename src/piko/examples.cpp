#include "examples.h"
#include "scripting/scripting.h"
#include <ek/canvas.h>
#include <ek/scenex/app/base_game.h>

namespace ek::piko {

void draw_book(entity_t e) {
    (void) e;
    js_call_global("draw_book");
}

void draw_dna(entity_t e) {
    (void) e;
    js_call_global("draw_dna");
}

void draw_diamonds(entity_t e) {
    auto* d = ecs::get<diamonds>(e);
    if (!d->rt.id) {
        d->start();
    }

    const sg_image_desc info = sg_query_image_desc(d->rt);
    canvas_set_image(d->rt);
    canvas_set_image_rect(rect_01());
    canvas_quad(0, 0, (float) info.width, (float) info.height);
//    recorder.render();
}

diamonds::diamonds() = default;

diamonds::~diamonds() {
    if (pass.id) sg_destroy_pass(pass);
    if (rt.id) {
        sg_destroy_image(rt);
    }
}

void diamonds::start() {
    //        recorder{"result", {0, 0, 512 * 2 / 2, 512 * 2 / 2}}
    rt = ek_gfx_make_render_target(128, 128, nullptr);
    sg_pass_desc passDesc{};
    passDesc.color_attachments[0].image = rt;
    passDesc.label = "diamonds-rt-pass";
    pass = sg_make_pass(passDesc);
}

}


void pre_render_diamonds(void) {
    for (entity_t e: ecs::view<ek::piko::diamonds>()) {
        ek::piko::diamonds* d = ecs::get<ek::piko::diamonds>(e);
        sg_image rt = d->rt;
        if (rt.id) {
            const sg_image_desc info = sg_query_image_desc(rt);
            int w = info.width;
            int h = info.height;

            sg_pass_action clear{};
            if (d->first_frame) {
                clear.colors[0].action = SG_ACTION_CLEAR;
                clear.colors[0].value.r = 0;
                clear.colors[0].value.g = 0;
                clear.colors[0].value.b = 0;
                clear.colors[0].value.a = 1;
                d->first_frame = false;
            } else {
                clear.colors[0].action = SG_ACTION_DONTCARE;
            }
            sg_begin_pass(d->pass, clear);
            canvas_begin_ex(rect_wh((float) w, (float) h), mat3x2_identity(), rt, {0});
            js_call_global("draw_diamonds");
            canvas_end();
            sg_end_pass();
        }
    }
}


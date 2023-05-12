#include "examples.h"
#include "scripting/scripting.h"
#include <ek/canvas.h>
#include <ek/scenex/app/base_game.h>

void draw_book(entity_t e) {
    (void)e;
    js_call_global("draw_book");
}

void draw_dna(entity_t e) {
    (void)e;
    js_call_global("draw_dna");
}

struct {
    sg_image rt;
    sg_pass pass;
    bool cleared;
} diamonds;

static
void start_diamonds(void) {
    diamonds.rt = ek_gfx_make_render_target(128, 128, NULL);
    sg_pass_desc desc = INIT_ZERO;
    desc.color_attachments[0].image = diamonds.rt;
    desc.label = "diamonds-rt";
    diamonds.pass = sg_make_pass(&desc);
}

void stop_diamonds(void) {
    if (diamonds.pass.id) {
        sg_destroy_pass(diamonds.pass);
        diamonds.pass.id = 0;
    }
    if (diamonds.rt.id) {
        sg_destroy_image(diamonds.rt);
        diamonds.rt.id = 0;
    }
    diamonds.cleared = false;
}

void draw_diamonds(entity_t e) {
    UNUSED(e);

    if (!diamonds.rt.id) {
        start_diamonds();
    }

    const sg_image_desc info = sg_query_image_desc(diamonds.rt);
    canvas_set_image(diamonds.rt);
    canvas_set_image_rect(rect_01());
    canvas_quad(0, 0, (float)info.width, (float)info.height);
}

void pre_render_diamonds(void) {
    sg_image rt = diamonds.rt;
    if (rt.id) {
        const sg_image_desc info = sg_query_image_desc(rt);
        int w = info.width;
        int h = info.height;

        sg_pass_action action = INIT_ZERO;
        if (!diamonds.cleared) {
            action.colors[0].action = SG_ACTION_CLEAR;
            action.colors[0].value.a = 1;
            diamonds.cleared = true;
        } else {
            action.colors[0].action = SG_ACTION_DONTCARE;
        }
        sg_begin_pass(diamonds.pass, &action);
        canvas_begin_ex(rect_wh((float)w, (float)h), mat3x2_identity(), rt, (sg_image){0});
        js_call_global("draw_diamonds");
        canvas_end();
        sg_end_pass();
    }
}

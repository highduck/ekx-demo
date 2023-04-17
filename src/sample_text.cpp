#include "sample_text.hpp"

#include <ek/scenex/text/text_engine.h>
#include <ek/scenex/text/font.h>
#include <ek/scenex/app/basic_application.hpp>
#include <ek/scenex/scene_factory.h>
#include <ek/scenex/base/node.h>
#include <ek/scenex/2d/layout_rect.h>
#include <ek/scenex/2d/text2d.h>
#include <ek/scenex/2d/display2d.h>
#include <piko/examples.h>

namespace ek {

entity_t createText(string_hash_t name, string_hash_t font, const char* text) {
    entity_t e = create_node2d(name);
    text2d_t* tf = text2d_setup(e);

    tf->format.font = R_FONT(font);
    tf->format.size = 48.0f;
    tf->format.wordWrap = true;

    // primary
    tf->format.layers[0].color = RGB(0xFFFF77);

    // inner stroke
    tf->format.layers[1].type = TEXT_LAYER_STROKE1;
    tf->format.layers[1].blurRadius = 1;
    tf->format.layers[1].blurIterations = 3;
    tf->format.layers[1].strength = 5;
    tf->format.layers[1].color = RGB(0x330000);
    tf->format.layers[1].visible = true;

    // outer stroke
    tf->format.layers[2].type = TEXT_LAYER_STROKE2;
    tf->format.layers[2].blurRadius = 2;
    tf->format.layers[2].blurIterations = 3;
    tf->format.layers[2].strength = 5;
    tf->format.layers[2].color = RGB(0xFFFFFF);
    tf->format.layers[2].visible = true;

    // shadow
    tf->format.layers[3].type = TEXT_LAYER_SHADOW;
    tf->format.layers[3].blurRadius = 8;
    tf->format.layers[3].blurIterations = 3;
    tf->format.layers[3].strength = 1;
    tf->format.layers[3].offset = vec2(4, 4);
    tf->format.layers[3].color = COLOR_BLACK;
    tf->format.layers[3].visible = true;

    tf->format.layersCount = 4;

    set_text(e, text);

    return e;
}

entity_t createScreenZones() {
    rect_t resolution = rect_wh(360, 480);
    auto zones = create_node2d(H("zones"));
    auto e = create_node2d(H("zone"));
    set_gradient_quad(e, resolution, COLOR_WHITE, ARGB(0x77FFFFFF));
    get_transform2d(e)->color.scale = ARGB(0x33FF00FF);
    layout_rect_t* l = add_layout_rect(e);
    LayoutRect_fill(l, true, true);
    l->doSafeInsets = true;
    append(zones, e);

    e = create_node2d(H("safe_zone"));
    set_gradient_quad(e, resolution, COLOR_WHITE, ARGB(0x77FFFFFF));
    get_transform2d(e)->color.scale = ARGB(0x3300FF00);
    append(zones, e);
    return zones;
}

SampleText::SampleText() :
        SampleBase() {
    title = "TEXT";

    append(container, createScreenZones());

    {
        entity_t bmText = createText(H("bmfont"), H("TickingTimebombBB"),
                                     "88:88:88\n-=98");
        text2d_t* txt = get_text2d(bmText);
        text_format_align(&txt->format, ALIGNMENT_CENTER);
        txt->format.size = 24;
        txt->borderColor = COLOR_BLACK;
        txt->rect = {{0, 0, 360 - 40, 100}};

        set_position(bmText, vec2(20, 20));
        append(container, bmText);
    }

    {
        entity_t ttfText = createText(H("TTF-Fallback"), H("Comfortaa-Regular"),
                                      u8"£ü÷\n< Приветики >\n你好\nनमस्कार\nこんにちは");
        text2d_t* txt = get_text2d(ttfText);
        text_format_align(&txt->format, ALIGNMENT_RIGHT | ALIGNMENT_TOP);
        text_format_set_text_color(&txt->format, ARGB(0xFF00FF00));
        txt->format.leading = -8;

        set_position(ttfText, vec2(360 - 20, 120));
        append(container, ttfText);
    }
    {
        entity_t ttfText = createText(H("TTF-Comfortaa-Regular"), H("Comfortaa-Regular"),
                                       u8"I don't know KERN TABLE.\nНо кириллица тоже есть");
        text2d_t* txt = get_text2d(ttfText);
        text_format_set_text_color(&txt->format, ARGB(0xFFFF00FF));
        txt->format.size = 24;

        set_position(ttfText, vec2(20, 340));
        append(container, ttfText);
    }
}

void SampleText::prepareInternalResources() {
    R(Font) native = R_FONT(H("native"));
    font_t* device_font = &REF_RESOLVE(res_font, native);

    const float scale_factor = asset_manager.scale_factor;
    font_init_ttf(device_font, scale_factor, 48, H("default_glyph_cache"));
    font_load_device_font(device_font, "Arial Unicode MS");

    RES_NAME_RESOLVE(res_font, H("Comfortaa-Regular")).fallback = native;
}

void SampleText::update(float dt) {
    SampleBase::update(dt);
}

}
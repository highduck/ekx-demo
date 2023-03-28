#include "sample_text.hpp"

#include <ek/scenex/text/TextEngine.hpp>
#include <ek/scenex/text/TrueTypeFont.hpp>
#include <ek/scenex/app/basic_application.hpp>
#include <ek/scenex/scene_factory.h>
#include <ek/scenex/base/node.h>
#include <ek/scenex/2d/LayoutRect.hpp>
#include <ek/scenex/2d/Display2D.hpp>
#include <piko/examples.h>

namespace ek {

entity_t createText(string_hash_t name, string_hash_t font, const char* text) {
    entity_t e = create_node2d(name);
    auto* tf = text2d_setup(e);

    tf->format.font = R_FONT(font);
    tf->format.size = 48.0f;
    tf->format.wordWrap = true;

    // primary
    tf->format.layers[0].color = RGB(0xFFFF77);

    // inner stroke
    tf->format.layers[1].type = TextLayerType::Stroke1;
    tf->format.layers[1].blurRadius = 1;
    tf->format.layers[1].blurIterations = 3;
    tf->format.layers[1].strength = 5;
    tf->format.layers[1].color = RGB(0x330000);

    // outer stroke
    tf->format.layers[2].type = TextLayerType::Stroke2;
    tf->format.layers[2].blurRadius = 2;
    tf->format.layers[2].blurIterations = 3;
    tf->format.layers[2].strength = 5;
    tf->format.layers[2].color = RGB(0xFFFFFF);

    // shadow
    tf->format.layers[3].type = TextLayerType::Shadow;
    tf->format.layers[3].blurRadius = 8;
    tf->format.layers[3].blurIterations = 3;
    tf->format.layers[3].strength = 1;
    tf->format.layers[3].offset = vec2(4, 4);
    tf->format.layers[3].color = COLOR_BLACK;

    tf->format.layersCount = 4;

    set_text(e, text);

    return e;
}

entity_t createScreenZones() {
    rect_t resolution = rect_wh(360, 480);
    auto zones = create_node2d(H("zones"));
    auto e = create_node2d(H("zone"));
    auto* q = quad2d_setup(e);
    q->setGradientVertical(COLOR_WHITE, ARGB(0x77FFFFFF));
    q->rect = resolution;
    ecs::get<Transform2D>(e).color.scale = ARGB(0x33FF00FF);
    ecs::add<LayoutRect>(e).fill(true, true).doSafeInsets = true;
    append(zones, e);

    e = create_node2d(H("safe_zone"));
    q = quad2d_setup(e);
    q->setGradientVertical(COLOR_WHITE, ARGB(0x77FFFFFF));
    q->rect = resolution;
    ecs::get<Transform2D>(e).color.scale = ARGB(0x3300FF00);
    append(zones, e);
    return zones;
}

SampleText::SampleText() :
        SampleBase() {
    title = "TEXT";

    append(container, createScreenZones());

    auto bmText = createText(H("bmfont"), H("TickingTimebombBB"),
                             "88:88:88\n-=98");
    ecs::get<Text2D>(bmText).format.setAlignment(Alignment::Center);
    ecs::get<Text2D>(bmText).format.size = 24;
    ecs::get<Text2D>(bmText).borderColor = COLOR_BLACK;
    ecs::get<Text2D>(bmText).rect = {{0, 0, 360 - 40, 100}};

    set_position(bmText, vec2(20, 20));
    append(container, bmText);

    entity_t ttfText = createText(H("TTF-Fallback"), H("Comfortaa-Regular"),
                              u8"£ü÷\n< Приветики >\n你好\nनमस्कार\nこんにちは");
    ecs::get<Text2D>(ttfText).format.setAlignment(Alignment::Right | Alignment::Top);
    ecs::get<Text2D>(ttfText).format.leading = -8;
    ecs::get<Text2D>(ttfText).format.setTextColor(ARGB(0xFF00FF00));
    set_position(ttfText, vec2(360 - 20, 120));
    append(container, ttfText);

    entity_t ttfText2 = createText(H("TTF-Comfortaa-Regular"), H("Comfortaa-Regular"),
                               u8"I don't know KERN TABLE.\nНо кириллица тоже есть");
    ecs::get<Text2D>(ttfText2).format.setTextColor(ARGB(0xFFFF00FF));
    ecs::get<Text2D>(ttfText2).format.size = 24;
    set_position(ttfText2, vec2(20, 340));
    append(container, ttfText2);
}

void SampleText::prepareInternalResources() {
    const float scale_factor = g_game_app->asset_manager.scale_factor;
    auto* ttfFont = new TrueTypeFont(scale_factor, 48, H("default_glyph_cache"));
    ttfFont->loadDeviceFont("Arial Unicode MS");

    R(Font) native = R_FONT(H("native"));
    REF_RESOLVE(res_font, native).impl = ttfFont;

    RES_NAME_RESOLVE(res_font, H("Comfortaa-Regular")).fallback = native;
}

void SampleText::update(float dt) {
    SampleBase::update(dt);
}

}
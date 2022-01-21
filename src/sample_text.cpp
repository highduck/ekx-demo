#include "sample_text.hpp"

#include <ek/scenex/text/TextEngine.hpp>
#include <ek/scenex/text/TrueTypeFont.hpp>
#include <ek/scenex/app/basic_application.hpp>
#include <ek/scenex/asset2/Asset.hpp>
#include <ek/scenex/SceneFactory.hpp>
#include <ek/scenex/base/Node.hpp>
#include <ek/scenex/2d/LayoutRect.hpp>
#include <piko/examples.h>

namespace ek {

ecs::EntityApi createText(string_hash_t name, string_hash_t font, const char* text) {
    auto e = createNode2D(name);
    auto* tf = new Text2D();

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

    tf->text = text;
    e.assign<Display2D>().drawable.reset(tf);

    return e;
}

ecs::EntityApi createScreenZones() {
    rect_t resolution = rect_wh(360, 480);
    auto zones = createNode2D(H("zones"));
    auto e = createNode2D(H("zone"));
    auto* q = new Quad2D();
    q->setGradientVertical(COLOR_WHITE, ARGB(0x77FFFFFF));
    q->rect = resolution;
    e.assign<Display2D>(q);
    e.get<Transform2D>().color.scale = ARGB(0x33FF00FF);
    e.assign<LayoutRect>().fill(true, true).doSafeInsets = true;
    append(zones, e);
    e = createNode2D(H("safe_zone"));
    q = new Quad2D();
    q->setGradientVertical(COLOR_WHITE, ARGB(0x77FFFFFF));
    q->rect = resolution;
    e.assign<Display2D>(q);
    e.get<Transform2D>().color.scale = ARGB(0x3300FF00);
    append(zones, e);
    return zones;
}

SampleText::SampleText() :
        SampleBase() {
    title = "TEXT";

    append(container, createScreenZones());

    auto bmText = createText(H("bmfont"), H("TickingTimebombBB"),
                             "88:88:88\n-=98");
    getDrawable<Text2D>(bmText).format.setAlignment(Alignment::Center);
    getDrawable<Text2D>(bmText).format.size = 24;
    getDrawable<Text2D>(bmText).borderColor = COLOR_BLACK;
    getDrawable<Text2D>(bmText).rect = {{0, 0, 360 - 40, 100}};

    setPosition(bmText, vec2(20, 20));
    append(container, bmText);

    auto ttfText = createText(H("TTF-Cousine-Regular"), H("Cousine-Regular"),
                              u8"£ü÷\n< Приветики >\n你好\nनमस्कार\nこんにちは");
    getDrawable<Text2D>(ttfText).format.setAlignment(Alignment::Right | Alignment::Top);
    getDrawable<Text2D>(ttfText).format.leading = -8;
    getDrawable<Text2D>(ttfText).format.setTextColor(ARGB(0xFF00FF00));
    setPosition(ttfText, vec2(360 - 20, 120));
    append(container, ttfText);

    auto ttfText2 = createText(H("TTF-Comfortaa-Regular"), H("Comfortaa-Regular"),
                               u8"I don't know KERN TABLE.\nНо кириллица тоже есть");
    getDrawable<Text2D>(ttfText2).format.setTextColor(ARGB(0xFFFF00FF));
    getDrawable<Text2D>(ttfText2).format.size = 24;
    setPosition(ttfText2, vec2(20, 340));
    append(container, ttfText2);
}

void SampleText::prepareInternalResources() {
    AssetManager* am = Locator::ref<basic_application>().asset_manager_;
    auto* ttfFont = new TrueTypeFont(am->scale_factor, 48, H("default_glyph_cache"));
    ttfFont->loadDeviceFont("Arial Unicode MS");

    R(Font) native = R_FONT(H("native"));
    REF_RESOLVE(res_font, native).impl = ttfFont;

    RES_NAME_RESOLVE(res_font, H("Cousine-Regular")).fallback = native;
}

void SampleText::update(float dt) {
    SampleBase::update(dt);
}

}
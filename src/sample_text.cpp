#include "sample_text.hpp"

#include <string>
#include <ek/scenex/text/TextEngine.hpp>
#include <ek/scenex/text/TrueTypeFont.hpp>
#include <ek/scenex/app/basic_application.hpp>
#include <ek/scenex/asset2/Asset.hpp>
#include <ek/scenex/SceneFactory.hpp>
#include <ek/scenex/base/Node.hpp>
#include <ek/scenex/2d/LayoutRect.hpp>
#include <piko/examples.h>

namespace ek {

ecs::EntityApi createText(const char* name, const char* font, const char* text) {
    auto e = createNode2D(name);
    auto* tf = new Text2D();

    tf->format.font.setID(font);
    tf->format.size = 48.0f;
    tf->format.wordWrap = true;

    // primary
    tf->format.layers[0].color = 0xFFFF77_rgb;

    // inner stroke
    tf->format.layers[1].type = TextLayerType::Stroke1;
    tf->format.layers[1].blurRadius = 1;
    tf->format.layers[1].blurIterations = 3;
    tf->format.layers[1].strength = 5;
    tf->format.layers[1].color = 0x330000_rgb;

    // outer stroke
    tf->format.layers[2].type = TextLayerType::Stroke2;
    tf->format.layers[2].blurRadius = 2;
    tf->format.layers[2].blurIterations = 3;
    tf->format.layers[2].strength = 5;
    tf->format.layers[2].color = 0xFFFFFF_rgb;

    // shadow
    tf->format.layers[3].type = TextLayerType::Shadow;
    tf->format.layers[3].blurRadius = 8;
    tf->format.layers[3].blurIterations = 3;
    tf->format.layers[3].strength = 1;
    tf->format.layers[3].offset = {4, 4};
    tf->format.layers[3].color = 0x0_rgb;

    tf->format.layersCount = 4;

    tf->text = text;
    e.assign<Display2D>().drawable.reset(tf);

    return e;
}

ecs::EntityApi createScreenZones() {
    rect_f resolution{0, 0, 360, 480};
    auto zones = createNode2D("zones");
    auto e = createNode2D("zone");
    auto* q = new Quad2D();
    q->setGradientVertical(0xFFFFFFFF_argb, 0x77FFFFFF_argb);
    q->rect = resolution;
    e.assign<Display2D>(q);
    e.get<Transform2D>().color.scale = 0x33FF00FF_argb;
    e.assign<LayoutRect>().fill(true, true).doSafeInsets = true;
    append(zones, e);
    e = createNode2D("safe_zone");
    q = new Quad2D();
    q->setGradientVertical(0xFFFFFFFF_argb, 0x77FFFFFF_argb);
    q->rect = resolution;
    e.assign<Display2D>(q);
    e.get<Transform2D>().color.scale = 0x3300FF00_argb;
    append(zones, e);
    return zones;
}

SampleText::SampleText() :
        SampleBase() {
    title = "TEXT";

    append(container, createScreenZones());

    auto bmText = createText("bmfont", "TickingTimebombBB",
                             "88:88:88\n-=98");
    getDrawable<Text2D>(bmText).format.setAlignment(Alignment::Center);
    getDrawable<Text2D>(bmText).format.size = 24;
    getDrawable<Text2D>(bmText).borderColor = 0xFF000000_argb;
    getDrawable<Text2D>(bmText).rect.set(0, 0, 360 - 40, 100);

    setPosition(bmText, {20.0f, 20.0f});
    append(container, bmText);

    auto ttfText = createText("TTF-Cousine-Regular", "Cousine-Regular",
                              u8"£ü÷\n< Приветики >\n你好\nनमस्कार\nこんにちは");
    getDrawable<Text2D>(ttfText).format.setAlignment(Alignment::Right | Alignment::Top);
    getDrawable<Text2D>(ttfText).format.leading = -8;
    getDrawable<Text2D>(ttfText).format.setTextColor(0xFF00FF00_argb);
    setPosition(ttfText, {360 - 20, 120.0f});
    append(container, ttfText);

    auto ttfText2 = createText("TTF-Comfortaa-Regular", "Comfortaa-Regular",
                               u8"I don't know KERN TABLE.\nНо кириллица тоже есть");
    getDrawable<Text2D>(ttfText2).format.setTextColor(0xFFFF00FF_argb);
    getDrawable<Text2D>(ttfText2).format.size = 24;
    setPosition(ttfText2, {20.0f, 340.0f});
    append(container, ttfText2);
}

void SampleText::prepareInternalResources() {
    AssetManager* am = Locator::ref<basic_application>().asset_manager_;
    auto* ttfFont = new TrueTypeFont(am->scale_factor, 48, "default_glyph_cache");
    ttfFont->loadDeviceFont("Arial Unicode MS");
    auto* font = new Font(ttfFont);
    Res<Font>{"native"}.reset(font);
    Res<Font>{"Cousine-Regular"}->setFallbackFont(font);
}

void SampleText::update(float dt) {
    SampleBase::update(dt);
}

}
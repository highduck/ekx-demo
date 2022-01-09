#include "minimal.hpp"
#include <ek/scenex/SceneFactory.hpp>
#include <ek/scenex/base/Interactive.hpp>
#include <ek/scenex/2d/Button.hpp>
#include <ek/scenex/2d/Display2D.hpp>

namespace ek {

Text2D& addText(ecs::EntityApi e, const char* text) {
    auto* tf = new Text2D();
    tf->format.font.setID("mini");
    tf->format.size = 14;
    tf->format.addShadow(COLOR_BLACK, 8);
    tf->text = text;
    tf->format.setAlignment(Alignment::Center);
    e.get_or_create<Display2D>().drawable.reset(tf);
    return *tf;
}

ecs::EntityApi createButton(const char* label, const std::function<void()>& fn) {
    auto e = createNode2D(label);
    auto& tf = addText(e, label);
    tf.fillColor = ARGB(0x77000000);
    tf.borderColor = ARGB(0x77FFFFFF);
    tf.hitFullBounds = true;
    tf.rect = {{-20, -20, 40, 40}};
    e.assign<Interactive>().cursor = EK_MOUSE_CURSOR_BUTTON;
    e.assign<Button>().clicked += fn;
    return e;
}

}
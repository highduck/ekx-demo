#include "minimal.hpp"
#include "ek/scenex/base/NodeEvents.hpp"
#include <ek/scenex/SceneFactory.hpp>
#include <ek/scenex/base/Interactive.hpp>
#include <ek/scenex/2d/Button.hpp>
#include <ek/scenex/2d/Display2D.hpp>

namespace ek {

Text2D& addText(ecs::Entity e, const char* text) {
    auto* tf = text2d_setup(e);
    tf->format.font = R_FONT(H("mini"));
    tf->format.size = 14;
    tf->format.addShadow(COLOR_BLACK, 8);
    tf->format.setAlignment(Alignment::Center);
    set_text(e, text);
    return *tf;
}

ecs::Entity createButton(const char* label, const std::function<void()>& fn) {
    auto e = createNode2D(H(label));
    auto& tf = addText(e, label);
    tf.fillColor = ARGB(0x77000000);
    tf.borderColor = ARGB(0x77FFFFFF);
    tf.hitFullBounds = true;
    tf.rect = {{-20, -20, 40, 40}};
    ecs::add<Interactive>(e).cursor = EK_MOUSE_CURSOR_BUTTON;
    ecs::add<Button>(e);
    ecs::add<NodeEventHandler>(e).on(BUTTON_EVENT_CLICK, [fn](const NodeEventData&) {
        fn();
    });
    return e;
}

}
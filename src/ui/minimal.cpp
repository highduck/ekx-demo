#include "minimal.hpp"
#include "ek/scenex/base/NodeEvents.hpp"
#include <ek/scenex/SceneFactory.hpp>
#include <ek/scenex/base/Interactive.hpp>
#include <ek/scenex/2d/Button.hpp>
#include <ek/scenex/2d/Display2D.hpp>

namespace ek {

Text2D& addText(ecs::EntityApi e, const char* text) {
    auto* tf = text2d_setup(e.index);
    tf->format.font = R_FONT(H("mini"));
    tf->format.size = 14;
    tf->format.addShadow(COLOR_BLACK, 8);
    tf->text = text;
    tf->format.setAlignment(Alignment::Center);
    return *tf;
}

ecs::EntityApi createButton(const char* label, const std::function<void()>& fn) {
    auto e = createNode2D(H(label));
    auto& tf = addText(e, label);
    tf.fillColor = ARGB(0x77000000);
    tf.borderColor = ARGB(0x77FFFFFF);
    tf.hitFullBounds = true;
    tf.rect = {{-20, -20, 40, 40}};
    e.assign<Interactive>().cursor = EK_MOUSE_CURSOR_BUTTON;
    e.assign<Button>();
    e.assign<NodeEventHandler>().on(BUTTON_EVENT_CLICK, [fn](const NodeEventData& event) {
        fn();
    });
    return e;
}

}
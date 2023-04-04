#include "minimal.hpp"
#include "ek/scenex/base/NodeEvents.hpp"
#include <ek/scenex/scene_factory.h>
#include <ek/scenex/base/interactiv.h>
#include <ek/scenex/2d/Button.hpp>
#include <ek/scenex/2d/Display2D.hpp>

namespace ek {

Text2D& addText(entity_t e, const char* text) {
    auto* tf = text2d_setup(e);
    tf->format.font = R_FONT(H("mini"));
    tf->format.size = 14;
    tf->format.addShadow(COLOR_BLACK, 8);
    tf->format.setAlignment(ALIGNMENT_CENTER);
    set_text(e, text);
    return *tf;
}

entity_t createButton(const char* label, const std::function<void()>& fn) {
    entity_t e = create_node2d(H(label));
    auto& tf = addText(e, label);
    tf.fillColor = ARGB(0x77000000);
    tf.borderColor = ARGB(0x77FFFFFF);
    tf.hitFullBounds = true;
    tf.rect = {{-20, -20, 40, 40}};
    interactive_add(e)->cursor = EK_MOUSE_CURSOR_BUTTON;
    ecs::add<Button>(e);
    ecs::add<NodeEventHandler>(e).on(BUTTON_EVENT_CLICK, [fn](const NodeEventData&) {
        fn();
    });
    return e;
}

}
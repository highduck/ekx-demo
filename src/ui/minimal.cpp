#include "minimal.hpp"
#include "ek/scenex/base/node_events.h"
#include <ek/scenex/scene_factory.h>
#include <ek/scenex/base/interactive.h>
#include <ek/scenex/2d/button.h>
#include <ek/scenex/2d/text2d.h>

namespace ek {

text2d_t* addText(entity_t e, const char* text) {
    text2d_t* tf = text2d_setup(e);
    tf->format = text_format(H("mini"), 14);
    text_format_add_shadow(&tf->format, COLOR_BLACK, 8, vec2(0,0 ), 0.2f);
    text_format_align(&tf->format, ALIGNMENT_CENTER);
    set_text(e, text);
    return tf;
}

entity_t createButton(const char* label, const std::function<void()>& fn) {
    entity_t e = create_node2d(H(label));
    text2d_t* tf = addText(e, label);
    tf->fillColor = ARGB(0x77000000);
    tf->borderColor = ARGB(0x77FFFFFF);
    tf->rect = {{-20, -20, 40, 40}};
    interactive_add(e)->cursor = EK_MOUSE_CURSOR_BUTTON;
    add_button(e);
    ecs::add<NodeEventHandler>(e)->on(BUTTON_EVENT_CLICK, [fn](const node_event_t*) {
        fn();
    });
    return e;
}

}
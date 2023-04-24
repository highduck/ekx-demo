#include "minimal_ui.h"

#include <ek/scenex/scene_factory.h>
#include <ek/scenex/base/interactive.h>
#include <ek/scenex/2d/button.h>
#include <ek/scenex/2d/text2d.h>

text2d_t* add_text(entity_t e, const char* text) {
    text2d_t* tf = text2d_setup(e);
    tf->format = text_format(H("mini"), 14);
    text_format_add_shadow(&tf->format, COLOR_BLACK, 8, vec2(0,0 ), 0.2f);
    text_format_align(&tf->format, ALIGNMENT_CENTER);
    set_text(e, text);
    return tf;
}

entity_t create_button(const char* label, void(*onclick)(const node_event_t*)) {
    entity_t e = create_node2d(H(label));
    text2d_t* tf = add_text(e, label);
    tf->fillColor = ARGB(0x77000000);
    tf->borderColor = ARGB(0x77FFFFFF);
    tf->rect = rect(-20, -20, 40, 40);
    add_interactive(e)->cursor = EK_MOUSE_CURSOR_BUTTON;
    add_button(e);
    add_node_event_listener(e, BUTTON_EVENT_CLICK, onclick);
    return e;
}

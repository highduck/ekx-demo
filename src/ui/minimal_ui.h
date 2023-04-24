#ifndef DEMO_MINIMAL_UI_H
#define DEMO_MINIMAL_UI_H

#include <ecx/ecx.h>
#include <ek/scenex/2d/text2d.h>
#include <ek/scenex/base/node_events.h>

#ifdef __cplusplus
extern "C" {
#endif

text2d_t* add_text(entity_t e, const char* text);

entity_t create_button(const char* label, void(* onclick)(const node_event_t*));

#ifdef __cplusplus
}
#endif

#endif // DEMO_MINIMAL_UI_H

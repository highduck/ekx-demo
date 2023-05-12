#pragma once

#include <ek/gfx.h>
#include <ecx/ecx.h>

#ifdef __cplusplus
extern "C" {
#endif

void draw_book(entity_t e);
void draw_dna(entity_t e);
void draw_diamonds(entity_t e);
void stop_diamonds(void);

void pre_render_diamonds(void);

#ifdef __cplusplus
}
#endif


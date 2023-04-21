#pragma once

#include <ek/gfx.h>
#include <ecx/ecx.hpp>

namespace ek::piko {

void draw_book(entity_t e);
void draw_dna(entity_t e);

struct diamonds {
    diamonds();

    ~diamonds();

    void start();

    sg_image rt{};
    sg_pass pass{};
    bool first_frame = true;
};

void draw_diamonds(entity_t e);

}

void pre_render_diamonds(void);

ECX_COMP_TYPE_CXX(ek::piko::diamonds)

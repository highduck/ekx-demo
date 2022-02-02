#pragma once

#include <ek/gfx.h>
#include <ecxx/ecxx.hpp>
#include <ek/scenex/app/GameAppListener.hpp>

namespace ek::piko {

void draw_book(entity_t e);
void draw_dna(entity_t e);

struct diamonds : public GameAppListener {
    diamonds();

    ~diamonds() override;

    void onPreRender() override;

    void start();

    sg_image rt{};
    sg_pass pass{};
    float time = 0.0f;
    bool first_frame = true;
};

void draw_diamonds(entity_t e);

}

ECX_COMP_TYPE_CXX(ek::piko::diamonds)

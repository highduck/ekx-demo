#pragma once

#include <ek/scenex/base/Script.hpp>
#include <ek/gfx.h>
#include <ek/util/Signal.hpp>
#include <ek/scenex/app/GameAppListener.hpp>

namespace ek::piko {

EK_DECL_SCRIPT_CPP(book) {
public:
    void draw() override;
};

EK_DECL_SCRIPT_CPP(dna) {
public:
    void draw() override;
};

EK_DECL_SCRIPT_CPP(diamonds), public GameAppListener {
public:
    diamonds();

    ~diamonds() override;

    void onPreRender() override;

    void draw() override;

    sg_image rt{};
    sg_pass pass{};
//    screen_recorder recorder;
    float time = 0.0f;
    bool first_frame = true;
};

}

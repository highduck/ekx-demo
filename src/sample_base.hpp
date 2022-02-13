#pragma once

#include <ecx/ecx.hpp>

namespace ek {

class SampleBase {
public:
    SampleBase();

    virtual ~SampleBase();

    virtual void draw();

    virtual void update(float dt);

    const char* title = nullptr;
    static ecs::Entity samplesContainer;
    ecs::Entity container;
private:
};

}


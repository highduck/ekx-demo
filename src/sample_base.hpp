#pragma once

#include <string>
#include <ecxx/ecxx.hpp>

namespace ek {

class SampleBase {
public:
    SampleBase();

    virtual ~SampleBase();

    virtual void draw();

    virtual void update(float dt);

    std::string title;
    static ecs::EntityApi samplesContainer;
    ecs::EntityApi container;
private:
};

}


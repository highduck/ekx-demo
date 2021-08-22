#pragma once

#include <sample_base.hpp>

namespace ek {

class SampleSim : public SampleBase {
    ecs::EntityApi countLabel;
    ecs::EntityApi particlesContainer;
    unsigned particlesCount = 0;

    void updateCountLabel() const;
public:
    SampleSim();
    ~SampleSim() override = default;

    void draw() override;

    void update(float dt) override;
};

}

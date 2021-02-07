#pragma once

#include <sample_base.hpp>

namespace ek {

class SampleSim : public SampleBase {
    ecs::entity countLabel;
    ecs::entity particlesContainer;
    unsigned particlesCount = 0;

    void updateCountLabel() const;
public:
    SampleSim();

    void draw() override;

    void update(float dt) override;
};

}

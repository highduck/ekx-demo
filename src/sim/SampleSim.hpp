#pragma once

#include <sample_base.hpp>

namespace ek {

class SampleSim : public SampleBase {
    ecs::Entity countLabel;
    ecs::Entity particlesContainer;
    unsigned particlesCount = 0;

    void updateCountLabel() const;
public:
    SampleSim();
    ~SampleSim() override = default;

    void draw() override;

    void update(float dt) override;
};

}

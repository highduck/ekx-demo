#pragma once

#include <sample_base.hpp>

namespace ek {

class SampleSim : public SampleBase {
    entity_t count_label;
    entity_t particles_container;
    uint32_t particles_count = 0;

    void updateCountLabel() const;
public:
    SampleSim();
    ~SampleSim() override;

    void draw() override;

    void update(float dt) override;
};

}

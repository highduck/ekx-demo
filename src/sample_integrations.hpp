#pragma once

#include "sample_base.hpp"

namespace ek {

class SampleIntegrations: public SampleBase {
public:
    SampleIntegrations();
    ~SampleIntegrations() override = default;
};

}

void sample_plugins_setup();

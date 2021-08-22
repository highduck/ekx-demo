#pragma once


#include "sample_base.hpp"

namespace ek {

class SampleIntegrations: public SampleBase {
public:
    SampleIntegrations();
    ~SampleIntegrations() override = default;

    static void initializePlugins();
};

}


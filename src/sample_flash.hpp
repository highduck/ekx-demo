#pragma once

#include "sample_base.hpp"

namespace ek {

class SampleFlash : public SampleBase {
public:
    explicit SampleFlash(const char* scene, const char* title_);
    ~SampleFlash() override = default;
};

}


#pragma once

#include <ek/scenex/text/text_engine.h>
#include "sample_base.hpp"

namespace ek {

class SampleText : public SampleBase {
public:
    SampleText();
    ~SampleText() override = default;

    void update(float dt) override;

    static void prepareInternalResources();

public:
};

}


#include "sample_flash.hpp"

#include <ek/scenex/scene_factory.h>
#include <ek/scenex/base/node.h>

namespace ek {

SampleFlash::SampleFlash(const char* scene, const char* title_) :
        SampleBase() {
    title = title_;
    append(container, sg_create(H("tests"), H(scene)));
}
}
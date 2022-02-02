#include "sample_flash.hpp"

#include <ek/scenex/SceneFactory.hpp>
#include <ek/scenex/base/Node.hpp>

namespace ek {

SampleFlash::SampleFlash(const char* scene, const char* title_) :
        SampleBase() {
    title = title_;
    append(container, sg_create(H("tests"), H(scene)));
}
}
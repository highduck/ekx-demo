#include "sample_flash.hpp"

#include <ek/scenex/SceneFactory.hpp>
#include <ek/scenex/base/Node.hpp>

namespace ek {

SampleFlash::SampleFlash(const char* scene) :
        SampleBase() {
    title = scene;
    append(container, sg_create("tests", scene));
}
}
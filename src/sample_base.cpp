#include "sample_base.hpp"
#include <ek/scenex/SceneFactory.hpp>
#include <ek/scenex/base/Node.hpp>

namespace ek {

SampleBase::SampleBase() {
    container = createNode2D("sample content");
    append(SampleBase::samplesContainer, container);
}

SampleBase::~SampleBase() {
    destroyNode(container);
}

void SampleBase::draw() {}

void SampleBase::update(float dt) {
    (void)dt;
}

ecs::EntityApi SampleBase::samplesContainer{};

}
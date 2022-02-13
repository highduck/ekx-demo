#include "sample_base.hpp"
#include <ek/scenex/SceneFactory.hpp>
#include <ek/scenex/base/Node.hpp>

namespace ek {

SampleBase::SampleBase() {
    container = createNode2D(H("sample content"));
    append(SampleBase::samplesContainer, container);
}

SampleBase::~SampleBase() {
    destroy_node(container);
}

void SampleBase::draw() {}

void SampleBase::update(float dt) {
    (void)dt;
}

ecs::Entity SampleBase::samplesContainer{};

}
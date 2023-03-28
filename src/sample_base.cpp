#include "sample_base.hpp"
#include <ek/scenex/scene_factory.h>
#include <ek/scenex/base/node.h>

namespace ek {

SampleBase::SampleBase() {
    container = create_node2d(H("sample content"));
    append(SampleBase::samplesContainer, container);
}

SampleBase::~SampleBase() {
    destroy_node(container);
}

void SampleBase::draw() {}

void SampleBase::update(float dt) {
    (void)dt;
}

entity_t SampleBase::samplesContainer = NULL_ENTITY;

}
#include "sample_base.h"
#include <ek/scenex/base/node.h>
#include <ek/scenex/scene_factory.h>

entity_t samples_container;

void start_sample(sample_t* sample) {
    if (sample) {
        if (!sample->container.id) {
            sample->container = create_node2d(H("sample content"));
            append(samples_container, sample->container);
        }
        if (sample->start) {
            sample->start(sample->container);
        }
    }
}

void stop_sample(sample_t* sample) {
    if (sample) {
        if (sample->container.id) {
            destroy_node(sample->container);
            sample->container = NULL_ENTITY;
        }
        if (sample->stop) {
            sample->stop();
        }
    }
}

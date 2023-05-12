#include "sample_flash.h"

#include <ek/scenex/base/node.h>
#include <ek/scenex/scene_factory.h>

void start_sample_flash_1(entity_t container) {
    sg_create(H("tests"), H("test1"), container);
}

void start_sample_flash_2(entity_t container) {
    sg_create(H("tests"), H("test2"), container);
}

sample_t sample_flash1(void) {
    sample_t s = INIT_ZERO;
    s.title = "TEST 1";
    s.start = start_sample_flash_1;
    return s;
}

sample_t sample_flash2(void) {
    sample_t s = INIT_ZERO;
    s.title = "TEST 2";
    s.start = start_sample_flash_2;
    return s;
}

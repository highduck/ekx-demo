#pragma once

#include <ecx/ecx.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
     WIDTH = 360,
     HEIGHT = 480,
};

typedef struct {
     const char* title;
     entity_t container;
     void(*start)(entity_t e);
     void(*stop)(void);
     void(*draw)(void);
     void(*update)(void);
} sample_t;

extern entity_t samples_container;

void start_sample(sample_t* sample);
void stop_sample(sample_t* sample);

#ifdef __cplusplus
}
#endif


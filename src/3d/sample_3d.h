#pragma once

#include "../sample_base.h"
#include <ecx/ecx.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void test_rotation3d_t;

extern ECX_DEFINE_TYPE(test_rotation3d_t);

sample_t sample3d(void);

#ifdef __cplusplus
}
#endif

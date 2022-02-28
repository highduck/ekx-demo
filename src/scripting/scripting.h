

#ifndef EKX_SCRIPTING_H
#define EKX_SCRIPTING_H

#include "js_api.h"

#ifdef __cplusplus
extern "C" {
#endif

void load_script(const char* path);
void js_initialize(void);
void js_call_global(const char* name);

#ifdef __cplusplus
}
#endif
#endif //EKX_SCRIPTING_H

#ifndef MODEL_3D_UTILS_H
#define MODEL_3D_UTILS_H

#include <gen_sg.h>

#ifdef __cplusplus
extern "C" {
#endif

model3d_t create_cube(vec3_t position, vec3_t size, color_t color);

model3d_t create_plane(vec3_t position, vec2_t size, color_t color);

#ifdef __cplusplus
}
#endif

#endif // MODEL_3D_UTILS_H

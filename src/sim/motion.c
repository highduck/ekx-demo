#include "motion.h"
#include "../sample_base.h"

#include <ek/scenex/2d/transform2d.h>

ECX_DEFINE_TYPE(motion_t);
ECX_DEFINE_TYPE(attractor_t);

typedef struct {
    attractor_t props;
    vec2_t position;
} sttractor_state_t;

void update_motion_system(float dt) {
    sttractor_state_t attractors[10];
    uint32_t attractors_num = 0;

    ECX_FOR_DRAFT(attractor_t) {
        attractors[attractors_num++] = (sttractor_state_t){
           *(attractor_t*)get_component_data(&ECX_ID(attractor_t), h_attractor_t, 0),
           get_transform2d(get_entity(&ECX_ID(attractor_t), h_attractor_t))->pos
        };
    }

    const float dump_factor = expf(-6.0f * dt);
    const aabb2_t bounds = aabb2_from_rect(rect_wh(WIDTH, HEIGHT));
    ECX_FOR_DRAFT(motion_t) {
        motion_t* mot = (motion_t*)get_component_data(&ECX_ID(motion_t), h_motion_t, 0);
        entity_t e = get_entity(&ECX_ID(motion_t), h_motion_t);
        transform2d_t* tra = get_transform2d(e);

        vec2_t p = tra->pos;
        vec2_t v = mot->velocity;

        for (uint32_t i = 0; i < attractors_num; ++i) {
            const sttractor_state_t attractor = attractors[i];
            const vec2_t diff = sub_vec2(attractor.position, p);
            const float len = length_vec2(diff);
            const float factor = 1.0f - saturate(len / attractor.props.radius);
            v = add_vec2(v, scale_vec2(diff, dt * attractor.props.force * factor * factor / len));
        }

        p = add_vec2(p, scale_vec2(mot->velocity, dt));

        if (p.x < bounds.x0) {
            v.x = -v.x;
            p.x = bounds.x0;
        } else if (p.x > bounds.x1) {
            v.x = -v.x;
            p.x = bounds.x1;
        }
        if (p.y < bounds.y0) {
            v.y = -v.y;
            p.y = bounds.y0;
        } else if (p.y > bounds.y1) {
            v.y = -v.y;
            p.y = bounds.y1;
        }

        mot->velocity = scale_vec2(v, dump_factor);
        tra->pos = p;
    }
}


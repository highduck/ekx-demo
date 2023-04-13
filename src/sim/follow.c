#include "follow.h"

#include <ek/scenex/2d/camera2d.h>
#include <ek/scenex/2d/transform2d.h>
#include <ek/scenex/base/node.h>
#include <ek/scenex/interaction_system.h>

ecx_component_type FollowMouse;
ecx_component_type FollowTarget;

void FollowTarget_ctor(component_handle_t i) {
    follow_target_t f = {0};
    f.k = 0.1f;
    f.fixed_frame_rate = 60.0f;
    f.n = 1;
    f.reset_in = 100;
    f.integration = FOLLOW_INTEGRATION_EXP;
    ((follow_target_t*) FollowTarget.data[0])[i] = f;
}

void Follow_setup(void) {
    init_component_type(&FollowMouse, (ecx_component_type_decl) {
            "Follow Mouse", 8, 0, {0}
    });
    init_component_type(&FollowTarget, (ecx_component_type_decl) {
            "Follow Target", 8, 1, {sizeof(follow_target_t)}
    });
    FollowTarget.ctor = FollowTarget_ctor;
}

void Follow_update(float dt) {
    for (uint32_t i = 1; i < FollowMouse.size; ++i) {
        const entity_t e = get_entity(&FollowMouse, i);
        const entity_t parent = get_parent(e);
        if (parent.id) {
            const vec2_t camera_pointer = vec2_transform(
                    g_interaction_system.pointerScreenPosition_,
                    get_camera2d(main_camera)->screenToWorldMatrix
            );
            const vec2_t pos = global_to_local(parent, camera_pointer);
            set_position(e, pos);
        }
    }

    for (uint32_t i = 1; i < FollowTarget.size; ++i) {
        follow_target_t* data = (follow_target_t*)get_component_data(&FollowTarget, i, 0);
        ++data->counter;
        data->time_accum += dt;
        if (data->counter >= data->n) {
            const entity_t e = get_entity(&FollowTarget, i);
            transform2d_t* tr = get_transform2d(e);

            if (is_entity(data->target_entity)) {
                const entity_t parent = get_parent(e);
                if (parent.id) {
                    data->target = local_to_local(data->target_entity, parent, vec2(0,0));
                }
            } else {
                data->target_entity = NULL_ENTITY;
            }

            vec2_t current = sub_vec2(tr->pos, data->offset);
            if (data->integration == FOLLOW_INTEGRATION_EXP) {
                const float c = logf(1.0f - data->k) * data->fixed_frame_rate;
                current = add_vec2(current, scale_vec2(sub_vec2(data->target, current), (1.0f - exp(c * data->time_accum))));
            } else if (data->integration == FOLLOW_INTEGRATION_STEPS) {
                const float timeStep = 1.0f / data->fixed_frame_rate;
                while (data->time_accum >= timeStep) {
                    current = add_vec2(current, scale_vec2(sub_vec2(data->target, current), data->k));
                    data->time_accum -= timeStep;
                }
                current = add_vec2(current, scale_vec2(sub_vec2(data->target, current), data->k * (data->time_accum * data->fixed_frame_rate)));
            } else if (data->integration == FOLLOW_INTEGRATION_NONE) {
                current = add_vec2(current, scale_vec2(sub_vec2(data->target, current), data->k * (data->time_accum * data->fixed_frame_rate)));
            }

            tr->pos = add_vec2(data->offset, current);
            data->counter = 0;
            data->time_accum = 0.0f;
        }
    }
}

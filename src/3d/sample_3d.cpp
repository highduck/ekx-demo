#include <ek/scenex/3d/scene3d.h>
#include <ek/scenex/base/node.h>
#include <ekx/app/time_layers.h>

#include <ek/rnd.h>
#include <ek/print.h>
#include <ek/scenex/2d/camera2d.h>
#include <stb/stb_sprintf.h>
#include "model_utils.h"
#include "sample_3d.hpp"
#include "camera_arcball.hpp"

namespace ek {

static entity_t main_camera_3d{};
static entity_t main_scene_3d{};

static string_hash_t test_materials[4] = {
        H("test0"),
        H("test1"),
        H("test2"),
        H("test3"),
};

static string_hash_t test_models[4] = {
        H("torus"),
        H("monkey"),
        H("sphere"),
        H("cube"),
};

void create_test_material(string_hash_t name, color_t color, float roughness) {
    material3d_t m = material3d(color, 0.5f);
    m.roughness = roughness;
    RES_NAME_RESOLVE(res_material3d, name) = m;
}

void create_lights() {
    {
        entity_t e_dir_light = create_entity();
        add_light3d(e_dir_light)[0] = light3d(LIGHT_DIRECTIONAL);
        mesh_renderer_t* mr = add_mesh_renderer(e_dir_light);
        add_transform3d(e_dir_light);
        add_node(e_dir_light)->tag = H("light_dir");
        mr->mesh = H("sphere");
        mr->material = H("light_material");
        mr->cast_shadows = false;
        mr->receive_shadows = false;
        get_transform3d(e_dir_light)->scale = vec3(5.0f, 5.0f, 1.0f);
        get_transform3d(e_dir_light)->position = vec3(-50.0f, -85.0f, 40.0f);
        append(main_scene_3d, e_dir_light);
    }

    {
        entity_t e_light = create_entity();

        light3d_t* l = add_light3d(e_light);
        *l = light3d(LIGHT_POINT);
        l->diffuse = vec3(1, 1, 1);

        mesh_renderer_t* mr = add_mesh_renderer(e_light);
        add_transform3d(e_light);
        add_node(e_light)->tag = H("light");

        get_transform3d(e_light)->position = vec3(0.0f, 0.0f, 15.0f);
        mr->mesh = H("sphere");
        mr->material = H("light_material");
        mr->cast_shadows = false;
        mr->receive_shadows = false;
        append(main_scene_3d, e_light);
    }
}

void create_coordinate_system_gizmo() {
    static static_mesh_t cube_x = static_mesh(create_cube(vec3(0, 0, 0), vec3(1, 1, 1), RGB(0xFF0000)));
    static static_mesh_t cube_y = static_mesh(create_cube(vec3(0, 0, 0), vec3(1, 1, 1), RGB(0x00FF00)));
    static static_mesh_t cube_z = static_mesh(create_cube(vec3(0, 0, 0), vec3(1, 1, 1), RGB(0x0000FF)));

    entity_t e = create_entity();
    {
        add_node(e)->tag = H("cs");
        transform3d_t* t = add_transform3d(e);
        t->scale = vec3(5.0f, 5.0f, 5.0f);
        append(main_scene_3d, e);
    }

    const float axis_size = 5.0f;
    {
        entity_t node = create_entity();
        add_node(node);
        mesh_renderer_t* r = add_mesh_renderer(node);
        transform3d_t* t = add_transform3d(node);
        r->mesh_ptr = &cube_x;
        t->position = vec3(0.5f * axis_size, 0.0f, 0.0f);
        t->scale = vec3(axis_size, 0.1f, 0.1f);
        append(e, node);
    }

    {
        entity_t node = create_entity();
        add_node(node);
        mesh_renderer_t* r = add_mesh_renderer(node);
        transform3d_t* t = add_transform3d(node);
        r->mesh_ptr = &cube_y;
        t->position = vec3(0.0f, 0.5f * axis_size, 0.0f);
        t->scale = vec3(0.1f, axis_size, 0.1f);
        append(e, node);
    }

    {
        entity_t node = create_entity();
        add_node(node);
        mesh_renderer_t* r = add_mesh_renderer(node);
        transform3d_t* t = add_transform3d(node);
        r->mesh_ptr = &cube_z;
        t->position = vec3(0.0f, 0.0f, 0.5f * axis_size);
        t->scale = vec3(0.1f, 0.1f, axis_size);
        append(e, node);
    }
}

void Sample3D::update(float dt) {
    SampleBase::update(dt);
    updateCameraArcBall(dt);
}

void Sample3D::draw() {
    SampleBase::draw();

    const float dt = g_time_layers->dt;

    for (auto e: ecs::view<test_rotation_comp>()) {
        transform3d_t* tr = get_transform3d(e);
        tr->rotation.x += dt;
        if (tr->rotation.x > MATH_PI) {
            tr->rotation.x -= MATH_TAU;
        }
        tr->rotation.y += dt * 2;
        if (tr->rotation.y > MATH_PI) {
            tr->rotation.y -= MATH_TAU;
        }
    }

    if (main_scene_3d.id) {
        entity_t light = find(main_scene_3d, H("light"));
        if (light.id) {
            static float lt = 0.0f;
            lt += dt;
            get_transform3d(light)->position = vec3(
                    50.0f * cos(lt),
                    50.0f * sin(lt),
                    15.0f
            );
        }
    }
}

Sample3D::Sample3D() {
    title = "SCENE 3D";
    get_camera2d(main_camera)->clearColorEnabled = false;

    material3d_t light_material = material3d(RGB(0xCCCCCC), 0.5f);
    light_material.emission = vec3(1, 1, 1);
    RES_NAME_RESOLVE(res_material3d, H("light_material")) = light_material;

    create_test_material(H("test0"), RGB(0xFF0000), 0.05f);
    create_test_material(H("test1"), RGB(0x00FF00), 0.1f);
    create_test_material(H("test2"), RGB(0x00FFFF), 0.2f);
    create_test_material(H("test3"), RGB(0xFFFF00), 0.3f);
    create_test_material(H("ground"), RGB(0x77FF77), 0.01f);

    //    asset_t<static_mesh_t>{"torus"}.reset(new static_mesh_t(load_obj(get_resource_content("assets/torus.obj"))));
//    asset_t<static_mesh_t>{"monkey"}.reset(new static_mesh_t(load_obj(get_resource_content("assets/monkey.obj"))));
//    asset_t<static_mesh_t>{"sphere"}.reset(new static_mesh_t(load_obj(get_resource_content("assets/sphere.obj"))));
    RES_NAME_RESOLVE(res_mesh3d, H("cube")) = static_mesh(create_cube(vec3(0, 0, 0), vec3(1, 1, 1), COLOR_WHITE));

    main_scene_3d = create_entity();
    add_node(main_scene_3d)->tag = H("scene 3d");
    add_transform3d(main_scene_3d);

    main_camera_3d = create_entity();
    add_node(main_camera_3d)->tag = H("main camera");
    add_transform3d(main_camera_3d);
    ecs::add<CameraArcBall>(main_camera_3d);
    camera3d_t* c = add_camera3d(main_camera_3d);
    *c = camera3d();

    scene3d_render_system.scene = main_scene_3d;
    scene3d_render_system.camera = main_camera_3d;

    transform3d_t* camera_transform = get_transform3d(main_camera_3d);
    camera_transform->position = vec3(-100.0f, -100.0f, 100.0f);
    append(main_scene_3d, main_camera_3d);
    c->cubemap = R_IMAGE(H("skybox"));
    c->clear_color_enabled = false;

    create_lights();

    {
        entity_t e_ground = create_entity();
        mesh_renderer_t* r = add_mesh_renderer(e_ground);
        transform3d_t* ground_transform = add_transform3d(e_ground);
        add_node(e_ground);
        r->mesh = H("cube");
        r->material = H("ground");

        set_tag(e_ground, H("ground"));
        ground_transform->scale = vec3(250.0f, 250.0f, 10.0f);
        ground_transform->position = vec3(0.0f, 0.0f, -5.0f);
        append(main_scene_3d, e_ground);
    }

    for (int i = 0; i < 20; ++i) {
        entity_t es = create_entity();
        mesh_renderer_t* renderer = add_mesh_renderer(es);
        ecs::add<test_rotation_comp>(es);
        transform3d_t* t = add_transform3d(es);
        add_node(es);
        renderer->mesh = H("sphere");

        char mat_id[64];
        ek_snprintf(mat_id, 64, "rr%d", i);
        const float r = static_cast<float>(i) / 20.0f;
        create_test_material(H(mat_id),
                             RGB(0xFF0000),
                             lerp_f32(0.001f, 0.33f, r));
        renderer->material = H(mat_id);
        t->position = vec3(
                lerp_f32(-100.0f, 100.0f, r),
                0.0f,
                5.0f
        );
        t->scale = vec3(4, 4, 4);
        append(main_scene_3d, es);
    }
    for (int i = 0; i < 10; ++i) {
        entity_t e_cube = create_entity();
        mesh_renderer_t* renderer = add_mesh_renderer(e_cube);
        ecs::add<test_rotation_comp>(e_cube);
        transform3d_t* t = add_transform3d(e_cube);
        add_node(e_cube);
        renderer->mesh = test_models[random_n(4)];
        renderer->material = test_materials[random_n(4)];
        set_tag(e_cube, H("cube"));
        t->position = vec3(
                random_range_f(-50.0f, 50.0f),
                random_range_f(-50.0f, 50.0f),
                random_range_f(5.0f, 15.0f)
        );

        const float tor_scale = random_range_f(1.0f, 5.0f);
        t->scale = vec3(tor_scale, tor_scale, tor_scale);
        t->rotation = vec3(
                random_range_f(0.0f, 180.0f),
                random_range_f(0.0f, 180.0f),
                random_range_f(0.0f, 180.0f)
        );
        append(main_scene_3d, e_cube);
    }

    create_coordinate_system_gizmo();
}

Sample3D::~Sample3D() {
    destroy_node(main_scene_3d);
    main_scene_3d = NULL_ENTITY;
    get_camera2d(main_camera)->clearColorEnabled = true;
}

}
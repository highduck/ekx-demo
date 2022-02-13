#include <ek/scenex/3d/Material3D.hpp>
#include <ek/scenex/3d/Transform3D.hpp>
#include <ek/scenex/3d/Camera3D.hpp>
#include <ek/scenex/3d/Transform3D.hpp>
#include <ek/time.h>
#include <ek/scenex/3d/RenderSystem3D.hpp>
#include <ek/scenex/base/Node.hpp>
#include <ekx/app/time_layers.h>
#include <ek/scenex/SceneFactory.hpp>
#include <ek/scenex/3d/Light3D.hpp>
#include <ek/scenex/3d/StaticMesh.hpp>

#include <demo_main.hpp>
#include <ek/rnd.h>
#include <ek/scenex/2d/Camera2D.hpp>
#include <stb/stb_sprintf.h>
#include "sample_3d.hpp"
#include "camera_arcball.hpp"

namespace ek {

static ecs::Entity main_camera{};
static ecs::Entity main_scene_3d{};

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
    Material3D m = {};
    m.set_base_color(color);
    m.roughness = roughness;
    RES_NAME_RESOLVE(res_material3d, name) = m;
}

void create_lights() {
    auto e_dir_light = ecs::create<Node, Transform3D, Light3D, MeshRenderer>();
    set_tag(e_dir_light, H("light_dir"));
    ecs::get<Light3D>(e_dir_light).type = Light3DType::Directional;
    ecs::get<MeshRenderer>(e_dir_light).mesh = H("sphere");
    ecs::get<MeshRenderer>(e_dir_light).material = H("light_material");
    ecs::get<MeshRenderer>(e_dir_light).castShadows = false;
    ecs::get<MeshRenderer>(e_dir_light).receiveShadows = false;
    ecs::get<Transform3D>(e_dir_light).scale = vec3(5.0f, 5.0f, 1.0f);
    ecs::get<Transform3D>(e_dir_light).position = vec3(-50.0f, -85.0f, 40.0f);
    append(main_scene_3d, e_dir_light);

    auto e_light = ecs::create<Node, Transform3D, Light3D, MeshRenderer>();
    set_tag(e_light, H("light"));
    ecs::get<Transform3D>(e_light).position = vec3(0.0f, 0.0f, 15.0f);
    ecs::get<Light3D>(e_light).diffuse = vec3(1, 1, 1);
    ecs::get<MeshRenderer>(e_light).mesh = H("sphere");
    ecs::get<MeshRenderer>(e_light).material = H("light_material");
    ecs::get<MeshRenderer>(e_light).castShadows = false;
    ecs::get<MeshRenderer>(e_light).receiveShadows = false;
    append(main_scene_3d, e_light);
}

void create_coordinate_system_gizmo() {
    static auto cube_x = new StaticMesh(Model3D::createCube(vec3(0, 0, 0), vec3(1, 1, 1), RGB(0xFF0000)));
    static auto cube_y = new StaticMesh(Model3D::createCube(vec3(0, 0, 0), vec3(1, 1, 1), RGB(0x00FF00)));
    static auto cube_z = new StaticMesh(Model3D::createCube(vec3(0, 0, 0), vec3(1, 1, 1), RGB(0x0000FF)));

    auto e = ecs::create<Node, Transform3D>();
    set_tag(e, H("cs"));
    ecs::get<Transform3D>(e).scale = vec3(5.0f, 5.0f, 5.0f);
    append(main_scene_3d, e);

    auto axis_size = 5.0f;
    auto ax = ecs::create<Node, Transform3D, MeshRenderer>();
    ecs::get<MeshRenderer>(ax).meshPtr = cube_x;
    ecs::get<MeshRenderer>(ax).material = H("light_material");
    ecs::get<Transform3D>(ax).position = vec3(0.5f * axis_size, 0.0f, 0.0f);
    ecs::get<Transform3D>(ax).scale = vec3(axis_size, 0.1f, 0.1f);
    append(e, ax);

    auto ay = ecs::create<Node, Transform3D, MeshRenderer>();
    ecs::get<MeshRenderer>(ay).meshPtr = cube_y;
    ecs::get<MeshRenderer>(ay).material = H("light_material");
    ecs::get<Transform3D>(ay).position = vec3(0.0f, 0.5f * axis_size, 0.0f);
    ecs::get<Transform3D>(ay).scale = vec3(0.1f, axis_size, 0.1f);
    append(e, ay);

    auto az = ecs::create<Node, Transform3D, MeshRenderer>();
    ecs::get<MeshRenderer>(az).meshPtr = cube_z;
    ecs::get<MeshRenderer>(az).material = H("light_material");
    ecs::get<Transform3D>(az).position = vec3(0.0f, 0.0f, 0.5f * axis_size);
    ecs::get<Transform3D>(az).scale = vec3(0.1f, 0.1f, axis_size);
    append(e, az);
}

void Sample3D::update(float dt) {
    SampleBase::update(dt);
    updateCameraArcBall(dt);
}

void Sample3D::draw() {
    SampleBase::draw();

    const float dt = g_time_layers->dt;

    for (auto e: ecs::view<test_rotation_comp>()) {
        auto& tr = ecs::get<Transform3D>(e);
        tr.rotation.x += dt;
        if (tr.rotation.x > MATH_PI) {
            tr.rotation.x -= MATH_TAU;
        }
        tr.rotation.y += dt * 2;
        if (tr.rotation.y > MATH_PI) {
            tr.rotation.y -= MATH_TAU;
        }
    }

    if (main_scene_3d) {
        ecs::Entity light = find(main_scene_3d, H("light"));
        if (light) {
            static float lt = 0.0f;
            lt += dt;
            ecs::get<Transform3D>(light).position = vec3(
                    50.0f * cos(lt),
                    50.0f * sin(lt),
                    15.0f
            );
        }
    }
}

Sample3D::Sample3D() {
    title = "SCENE 3D";
    ecs::get<Camera2D>(Camera2D::Main).clearColorEnabled = false;

    Material3D light_material = {};
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
    RES_NAME_RESOLVE(res_mesh3d, H("cube")) = new StaticMesh(Model3D::createCube(vec3(0, 0, 0), vec3(1, 1, 1)));

    main_scene_3d = ecs::create<Node, Transform3D>();
    set_tag(main_scene_3d, H("scene 3d"));

    main_camera = ecs::create<Node, Camera3D, Transform3D, CameraArcBall>();
    set_tag(main_camera, H("main camera"));

    g_render_system_3d->scene = main_scene_3d;
    g_render_system_3d->camera = main_camera;

    auto& camera_transform = ecs::get<Transform3D>(main_camera);
    camera_transform.position = vec3(-100.0f, -100.0f, 100.0f);
    append(main_scene_3d, main_camera);
    ecs::get<Camera3D>(main_camera).cubeMap = R_IMAGE(H("skybox"));
    ecs::get<Camera3D>(main_camera).clearColorEnabled = false;

    create_lights();

    auto e_ground = ecs::create<Node, Transform3D, MeshRenderer>();
    ecs::get<MeshRenderer>(e_ground).mesh = H("cube");
    ecs::get<MeshRenderer>(e_ground).material = H("ground");

    set_tag(e_ground, H("ground"));
    ecs::get<Transform3D>(e_ground).scale = vec3(250.0f, 250.0f, 10.0f);
    ecs::get<Transform3D>(e_ground).position = vec3(0.0f, 0.0f, -5.0f);
    append(main_scene_3d, e_ground);

    for (int i = 0; i < 20; ++i) {
        auto es = ecs::create<Node, Transform3D, MeshRenderer, test_rotation_comp>();
        ecs::get<MeshRenderer>(es).mesh = H("sphere");

        char mat_id[64];
        ek_snprintf(mat_id, 64, "rr%d", i);
        auto r = static_cast<float>(i) / 20.0f;
        create_test_material(H(mat_id),
                             RGB(0xFF0000),
                             lerp_f32(0.001f, 0.33f, r));
        ecs::get<MeshRenderer>(es).material = H(mat_id);
        ecs::get<Transform3D>(es).position = vec3(
                lerp_f32(-100.0f, 100.0f, r),
                0.0f,
                5.0f
        );
        ecs::get<Transform3D>(es).scale = vec3(4, 4, 4);
        append(main_scene_3d, es);
    }
    for (int i = 0; i < 10; ++i) {
        auto e_cube = ecs::create<Node, Transform3D, MeshRenderer, test_rotation_comp>();
        ecs::get<MeshRenderer>(e_cube).mesh = test_models[random_n(4)];
        ecs::get<MeshRenderer>(e_cube).material = test_materials[random_n(4)];
        set_tag(e_cube, H("cube"));
        ecs::get<Transform3D>(e_cube).position = vec3(
                random_range_f(-50.0f, 50.0f),
                random_range_f(-50.0f, 50.0f),
                random_range_f(5.0f, 15.0f)
        );

        const float tor_scale = random_range_f(1.0f, 5.0f);
        ecs::get<Transform3D>(e_cube).scale = vec3(tor_scale, tor_scale, tor_scale);
        ecs::get<Transform3D>(e_cube).rotation = vec3(
                random_range_f(0.0f, 180.0f),
                random_range_f(0.0f, 180.0f),
                random_range_f(0.0f, 180.0f)
        );
        append(main_scene_3d, e_cube);
    }
}

Sample3D::~Sample3D() {
    destroy_node(main_scene_3d);
    main_scene_3d = nullptr;
    ecs::get<Camera2D>(Camera2D::Main).clearColorEnabled = true;
}

}
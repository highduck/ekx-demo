#include <ek/scenex/3d/Material3D.hpp>
#include <ek/scenex/3d/Transform3D.hpp>
#include <ek/scenex/3d/Camera3D.hpp>
#include <ek/scenex/3d/Transform3D.hpp>
#include <ek/time.h>
#include <ek/scenex/3d/RenderSystem3D.hpp>
#include <ek/scenex/base/Node.hpp>
#include <ek/scenex/base/TimeLayer.hpp>
#include <ek/scenex/SceneFactory.hpp>
#include <ek/scenex/3d/Light3D.hpp>
#include <ek/scenex/3d/StaticMesh.hpp>
#include <ek/util/Res.hpp>

#include <demo_main.hpp>
#include <ek/math/Random.hpp>
#include <ek/scenex/2d/Camera2D.hpp>
#include <stb/stb_sprintf.h>
#include "sample_3d.hpp"
#include "camera_arcball.hpp"

namespace ek {

static ecs::EntityApi main_camera{};
static ecs::EntityApi main_scene_3d{};

static const char* test_materials[4] = {"test0",
                                        "test1",
                                        "test2",
                                        "test3"};
static const char* test_models[4] = {"torus",
                                     "monkey",
                                     "sphere",
                                     "cube"};

void create_test_material(const char* name, color_t color, float roughness) {
    auto m = new Material3D;
    m->set_base_color(color);
    m->roughness = roughness;
    Res<Material3D>{name}.reset(m);
}

void create_lights() {
    auto e_dir_light = ecs::create<Node, Transform3D, Light3D, MeshRenderer>();
    setName(e_dir_light, "light_dir");
    e_dir_light.get<Light3D>().type = Light3DType::Directional;
    e_dir_light.get<MeshRenderer>().mesh = "sphere";
    e_dir_light.get<MeshRenderer>().material = "light_material";
    e_dir_light.get<MeshRenderer>().castShadows = false;
    e_dir_light.get<MeshRenderer>().receiveShadows = false;
    e_dir_light.get<Transform3D>().scale = vec3(5.0f, 5.0f, 1.0f);
    e_dir_light.get<Transform3D>().position = vec3(-50.0f, -85.0f, 40.0f);
    append(main_scene_3d, e_dir_light);

    auto e_light = ecs::create<Node, Transform3D, Light3D, MeshRenderer>();
    setName(e_light, "light");
    e_light.get<Transform3D>().position = vec3(0.0f, 0.0f, 15.0f);
    e_light.get<Light3D>().diffuse = vec3(1, 1, 1);
    e_light.get<MeshRenderer>().mesh = "sphere";
    e_light.get<MeshRenderer>().material = "light_material";
    e_light.get<MeshRenderer>().castShadows = false;
    e_light.get<MeshRenderer>().receiveShadows = false;
    append(main_scene_3d, e_light);
}

void create_coordinate_system_gizmo() {
    static auto cube_x = new StaticMesh(Model3D::createCube(vec3(0, 0, 0), vec3(1, 1, 1), RGB(0xFF0000)));
    static auto cube_y = new StaticMesh(Model3D::createCube(vec3(0, 0, 0), vec3(1, 1, 1), RGB(0x00FF00)));
    static auto cube_z = new StaticMesh(Model3D::createCube(vec3(0, 0, 0), vec3(1, 1, 1), RGB(0x0000FF)));

    auto e = ecs::create<Node, Transform3D>();
    setName(e, "cs");
    e.get<Transform3D>().scale = vec3(5.0f, 5.0f, 5.0f);
    append(main_scene_3d, e);

    auto axis_size = 5.0f;
    auto ax = ecs::create<Node, Transform3D, MeshRenderer>();
    ax.get<MeshRenderer>().meshPtr = cube_x;
    ax.get<MeshRenderer>().material = "light_material";
    ax.get<Transform3D>().position = vec3(0.5f * axis_size, 0.0f, 0.0f);
    ax.get<Transform3D>().scale = vec3(axis_size, 0.1f, 0.1f);
    append(e, ax);

    auto ay = ecs::create<Node, Transform3D, MeshRenderer>();
    ay.get<MeshRenderer>().meshPtr = cube_y;
    ay.get<MeshRenderer>().material = "light_material";
    ay.get<Transform3D>().position = vec3(0.0f, 0.5f * axis_size, 0.0f);
    ay.get<Transform3D>().scale = vec3(0.1f, axis_size, 0.1f);
    append(e, ay);

    auto az = ecs::create<Node, Transform3D, MeshRenderer>();
    az.get<MeshRenderer>().meshPtr = cube_z;
    az.get<MeshRenderer>().material = "light_material";
    az.get<Transform3D>().position = vec3(0.0f, 0.0f, 0.5f * axis_size);
    az.get<Transform3D>().scale = vec3(0.1f, 0.1f, axis_size);
    append(e, az);
}

void Sample3D::update(float dt) {
    SampleBase::update(dt);
    updateCameraArcBall(dt);
}

void Sample3D::draw() {
    SampleBase::draw();

    const float dt = TimeLayer::Root->dt;

    for (auto e: ecs::view<test_rotation_comp>()) {
        auto& tr = e.get<Transform3D>();
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
        auto light = find(main_scene_3d, "light");
        if (light) {
            static float lt = 0.0f;
            lt += dt;
            light.get<Transform3D>().position = vec3(
                    50.0f * cos(lt),
                    50.0f * sin(lt),
                    15.0f
            );
        }
    }
}

Sample3D::Sample3D() {
    auto* rs3d = Locator::get<RenderSystem3D>();

    title = "SCENE 3D";
    Camera2D::Main.get<Camera2D>().clearColorEnabled = false;

    auto light_material = new Material3D;
    light_material->emission = vec3(1, 1, 1);
    Res<Material3D>{"light_material"}.reset(light_material);

    create_test_material("test0", RGB(0xFF0000), 0.05f);
    create_test_material("test1", RGB(0x00FF00), 0.1f);
    create_test_material("test2", RGB(0x00FFFF), 0.2f);
    create_test_material("test3", RGB(0xFFFF00), 0.3f);
    create_test_material("ground", RGB(0x77FF77), 0.01f);

    //    asset_t<static_mesh_t>{"torus"}.reset(new static_mesh_t(load_obj(get_resource_content("assets/torus.obj"))));
//    asset_t<static_mesh_t>{"monkey"}.reset(new static_mesh_t(load_obj(get_resource_content("assets/monkey.obj"))));
//    asset_t<static_mesh_t>{"sphere"}.reset(new static_mesh_t(load_obj(get_resource_content("assets/sphere.obj"))));
    Res<StaticMesh>{"cube"}.reset(new StaticMesh(Model3D::createCube(vec3(0, 0, 0), vec3(1, 1, 1))));

    main_scene_3d = ecs::create<Node, Transform3D>();
    setName(main_scene_3d, "scene 3d");

    main_camera = ecs::create<Node, Camera3D, Transform3D, CameraArcBall>();
    setName(main_camera, "main camera");

    rs3d->scene = main_scene_3d;
    rs3d->camera = main_camera;

    auto& camera_transform = main_camera.get<Transform3D>();
    camera_transform.position = vec3(-100.0f, -100.0f, 100.0f);
    append(main_scene_3d, main_camera);
    main_camera.get<Camera3D>().cubeMap = ek_ref_find(sg_image, "skybox");
    main_camera.get<Camera3D>().clearColorEnabled = false;

    create_lights();

    auto e_ground = ecs::create<Node, Transform3D, MeshRenderer>();
    e_ground.get<MeshRenderer>().mesh = "cube";
    e_ground.get<MeshRenderer>().material = "ground";

    setName(e_ground, "ground");
    e_ground.get<Transform3D>().scale = vec3(250.0f, 250.0f, 10.0f);
    e_ground.get<Transform3D>().position = vec3(0.0f, 0.0f, -5.0f);
    append(main_scene_3d, e_ground);

    for (int i = 0; i < 20; ++i) {
        auto es = ecs::create<Node, Transform3D, MeshRenderer, test_rotation_comp>();
        es.get<MeshRenderer>().mesh = "sphere";

        char mat_id[64];
        ek_snprintf(mat_id, 64, "rr%d", i);
        auto r = static_cast<float>(i) / 20.0f;
        create_test_material(mat_id,
                             RGB(0xFF0000),
                             lerp_f32(0.001f, 0.33f, r));
        es.get<MeshRenderer>().material = mat_id;
        es.get<Transform3D>().position = vec3(
                lerp_f32(-100.0f, 100.0f, r),
                0.0f,
                5.0f
        );
        es.get<Transform3D>().scale = vec3(4, 4, 4);
        append(main_scene_3d, es);
    }
    for (int i = 0; i < 10; ++i) {
        auto e_cube = ecs::create<Node, Transform3D, MeshRenderer, test_rotation_comp>();
        e_cube.get<MeshRenderer>().mesh = test_models[rand_fx.random_int(0, 3)];
        e_cube.get<MeshRenderer>().material = test_materials[rand_fx.random_int(0, 3)];
        setName(e_cube, "cube");
        e_cube.get<Transform3D>().position = vec3(
                rand_fx.random(-50.0f, 50.0f),
                rand_fx.random(-50.0f, 50.0f),
                rand_fx.random(5.0f, 15.0f)
        );

        const float tor_scale = rand_fx.random(1.0f, 5.0f);
        e_cube.get<Transform3D>().scale = vec3(tor_scale, tor_scale, tor_scale);
        e_cube.get<Transform3D>().rotation = vec3(
                rand_fx.random(0.0f, 180.0f),
                rand_fx.random(0.0f, 180.0f),
                rand_fx.random(0.0f, 180.0f)
        );
        append(main_scene_3d, e_cube);
    }
}

Sample3D::~Sample3D() {
    destroyNode(main_scene_3d);
    main_scene_3d = nullptr;
    Camera2D::Main.get<Camera2D>().clearColorEnabled = true;
}

}
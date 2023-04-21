#include "sample_text.hpp"
#include "sample_flash.hpp"
#include "3d/sample_3d.hpp"
#include "piko/sample_piko.hpp"
#include "sim/SampleSim.hpp"
#include "sample_integrations.hpp"
#include "sample_audio.hpp"
#include "sim/Motion.hpp"
#include "sim/follow.h"
#include "3d/camera_arcball.hpp"
#include "piko/examples.h"
#include "config/build_info.h"
#include <ui/minimal.hpp>

#include <ek/firebase.h>
#include <ek/scenex/app/base_game.h>
#include <ek/scenex/systems/main_flow.h>
#include <ek/scenex/scene_factory.h>
#include <ek/scenex/base/node.h>
#include <ek/scenex/2d/layout_rect.h>
#include <ek/scenex/2d/camera2d.h>
#include <ek/scenex/2d/text2d.h>
#include <ek/log.h>
#include <ek/scenex/3d/scene3d.h>
#include <ekx/app/time_layers.h>
#include <appbox/appbox.h>
#include <ekx/app/profiler.h>

#ifdef EK_UITEST
#include "screenshots.hpp"
#endif

static void on_pre_load(void);

static void on_start(void);

static void on_update(void);

static void on_pre_render(void);

static void on_pre_render_scene(void);

static void on_terminate(void);

void ek_app_main() {
    firebase(FIREBASE_CMD_INIT);
    ek_app.config.title = "ekx";
    ek_app.config.width = 360;
    ek_app.config.height = 480;
    ek_app.config.need_depth = true;
    game_app_state.delegate.pre_load = on_pre_load;
    game_app_state.delegate.start = on_start;
    game_app_state.delegate.update = on_update;
    game_app_state.delegate.pre_render = on_pre_render;
    game_app_state.delegate.pre_render_scene = on_pre_render_scene;
    game_app_state.delegate.terminate = on_terminate;
    run_app();
}

namespace ek {

typedef SampleBase* (* sample_factory_fn)();

Array<sample_factory_fn> sampleFactory;
int currentSampleIndex = 0;
SampleBase* currentSample = nullptr;
entity_t tfSampleTitle;
entity_t tfFPS = NULL_ENTITY;
int prevFPS = 0;
fps_counter fps_cnt = {};

void activate_sample(int index);

void init_samples() {
    sampleFactory.push_back([]() -> SampleBase* { return new SamplePiko(); });
    sampleFactory.push_back([]() -> SampleBase* { return new SampleSim(); });
    sampleFactory.push_back([]() -> SampleBase* { return new SampleFlash("test1", "TEST 1"); });
    sampleFactory.push_back([]() -> SampleBase* { return new SampleFlash("test2", "TEST 2"); });
    sampleFactory.push_back([]() -> SampleBase* { return new Sample3D(); });
    sampleFactory.push_back([]() -> SampleBase* { return new SampleAudio(); });
    sampleFactory.push_back([]() -> SampleBase* { return new SampleIntegrations(); });
    sampleFactory.push_back([]() -> SampleBase* { return new SampleText(); });
    activate_sample(0);
}

void activate_sample(int index) {
    int samplesCount = static_cast<int>(sampleFactory.size());
    currentSampleIndex = index;
    if (currentSampleIndex >= samplesCount) {
        currentSampleIndex = 0;
    }
    if (currentSampleIndex < 0) {
        currentSampleIndex = samplesCount - 1;
    }
    delete currentSample;
    currentSample = sampleFactory[currentSampleIndex]();
    set_text(tfSampleTitle, currentSample->title);
}

void scroll_sample(int delta) {
    activate_sample(currentSampleIndex + delta);
}

}

using namespace ek;

static void on_pre_render() {
    scene3d_prerender();
    pre_render_diamonds();
}

static void on_pre_render_scene(void) {
    scene3d_render(&game_app_state.display.info);
    if (game_app_state.started && currentSample) {
        currentSample->draw();
    }
}

static void on_pre_load(void) {
#ifdef EK_UITEST
    ::uitest("screenshots", do_game_screenshots);
#endif

    scene3d_setup();

    ECX_COMPONENT(sim::motion_t);
    ECX_COMPONENT(sim::attractor_t);
    ECX_COMPONENT(CameraArcBall);
    ECX_COMPONENT(test_rotation_comp);
    Follow_setup();
    ECX_COMPONENT(piko::diamonds);

    camera2d_t* cam = get_camera2d(main_camera);
    cam->clearColorEnabled = true;
    cam->clearColor = vec4_color(ARGB(0xFF666666));

    sample_plugins_setup();
}

void on_update(void) {
    const float dt = g_time_dt;
    if (currentSample) {
        currentSample->update(dt);
    }
    if (tfFPS.id) {
        fps_counter_update(&fps_cnt, dt);
        auto fps = (int) fps_cnt.average;
        if (fps != prevFPS) {
            prevFPS = fps;
            set_text_f(tfFPS, "FPS: %d", fps);
        }
    }
}

static void on_start(void) {
    appbox_on_game_start();

    const entity_t root = game_app_state.root;
//    setup_game(w, game);
    log_debug("Start Demo: prepareInternalResources");
    SampleText::prepareInternalResources();

    SampleBase::samplesContainer = create_node2d(H("sample"));
    append(root, SampleBase::samplesContainer);

    entity_t prev = createButton("<", +[]() -> void { scroll_sample(-1); });
    entity_t next = createButton(">", +[]() -> void { scroll_sample(1); });
    LayoutRect_enableAlignX(add_layout_rect(prev), 0, 30);
    LayoutRect_enableAlignX(add_layout_rect(next), 1, -30);

    tfSampleTitle = create_node2d(H("title"));
    addText(tfSampleTitle, "");
    LayoutRect_enableAlignX(add_layout_rect(tfSampleTitle), 0.5, 0);

    {
        tfFPS = create_node2d(H("fps"));
        addText(tfFPS, "");
        text2d_t* tf = get_text2d(tfFPS);
        tf->format.alignment = vec2(0, 0);
        tf->format.size = 8.0f;
        layout_rect_t* ll = add_layout_rect(tfFPS);
        LayoutRect_enableAlignX(ll, 0.0, 10);
        LayoutRect_enableAlignY(ll, 0.0, 10);
        append(root, tfFPS);
    }


    {
        entity_t tf_version = create_node2d(H("version"));
        addText(tf_version, APP_VERSION_NAME "+" APP_VERSION_CODE);
        text2d_t* tf = get_text2d(tf_version);
        tf->format.alignment = vec2(1, 0);
        tf->format.size = 8.0f;
        layout_rect_t* ll = add_layout_rect(tf_version);
        LayoutRect_enableAlignX(ll, 1.0, -10);
        LayoutRect_enableAlignY(ll, 0.0, 10);
        append(root, tf_version);
    }

    entity_t controls = create_node2d(H("controls"));
    append(controls, tfSampleTitle);
    append(controls, prev);
    append(controls, next);
    LayoutRect_enableAlignY(add_layout_rect(controls), 1, -30);

    append(root, controls);

    log_debug("Start Demo: initSamples");
    init_samples();

    SampleAudio::startMusicTrack();
}

static void on_terminate(void) {
    delete currentSample;
    currentSample = nullptr;
}

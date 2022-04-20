#include <ek/firebase.h>

#include "demo_main.hpp"
#include "sample_text.hpp"
#include "sample_flash.hpp"
#include "3d/sample_3d.hpp"
#include "piko/sample_piko.hpp"
#include "sim/SampleSim.hpp"
#include "sample_integrations.hpp"
#include "sample_audio.hpp"
#include "sim/Motion.hpp"
#include "3d/camera_arcball.hpp"
#include "ek/goodies/helpers/follow.h"
#include "piko/examples.h"

#include <ek/rnd.h>
#include <ek/scenex/systems/main_flow.hpp>
#include <ek/scenex/asset2/Asset.hpp>
#include <ek/scenex/SceneFactory.hpp>
#include <ek/scenex/base/Interactive.hpp>
#include <ek/scenex/base/Node.hpp>
#include <ek/scenex/2d/LayoutRect.hpp>
#include <ui/minimal.hpp>
#include <ek/scenex/2d/Camera2D.hpp>
#include <ek/scenex/2d/Display2D.hpp>
#include <ek/log.h>
#include <ekx/app/audio_manager.h>
#include <ek/scenex/3d/Scene3D.h>
#include "config/build_info.h"

#ifdef EK_UITEST
#include "screenshots.hpp"
#endif

void ek_app_main() {
    firebase(FIREBASE_CMD_INIT);
    ek_app.config.title = "ekx";
    ek_app.config.width = 360;
    ek_app.config.height = 480;
    ek_app.config.need_depth = true;
    ek::run_app<DemoApp>();
}

namespace ek {

typedef SampleBase* (* sample_factory_fn)();

Array<sample_factory_fn> sampleFactory;
int currentSampleIndex = 0;
SampleBase* currentSample = nullptr;
ecs::Entity tfSampleTitle;
ecs::Entity tfFPS = nullptr;
int prevFPS = 0;
fps_counter fps_cnt = {};

void setCurrentSample(int index);

void initSamples() {
    sampleFactory.push_back([]() -> SampleBase* { return new SamplePiko(); });
    sampleFactory.push_back([]() -> SampleBase* { return new SampleSim(); });
    sampleFactory.push_back([]() -> SampleBase* { return new SampleFlash("test1", "TEST 1"); });
    sampleFactory.push_back([]() -> SampleBase* { return new SampleFlash("test2", "TEST 2"); });
    sampleFactory.push_back([]() -> SampleBase* { return new Sample3D(); });
    sampleFactory.push_back([]() -> SampleBase* { return new SampleAudio(); });
    sampleFactory.push_back([]() -> SampleBase* { return new SampleIntegrations(); });
    sampleFactory.push_back([]() -> SampleBase* { return new SampleText(); });
    setCurrentSample(0);
}

void setCurrentSample(int index) {
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

void scrollSample(int delta) {
    setCurrentSample(currentSampleIndex + delta);
}

}

using namespace ek;

DemoApp::DemoApp() :
        basic_application() {
#ifdef EK_UITEST
    uitest::UITest("screenshots", uitest::runScreenshotScript);
#endif
}

void DemoApp::initialize() {
    basic_application::initialize();

    initScene3D();

    ECX_COMPONENT(sim::motion_t);
    ECX_COMPONENT(sim::attractor_t);
    ECX_COMPONENT(CameraArcBall);
    ECX_COMPONENT(test_rotation_comp);
    ECX_COMPONENT(target_follow_comp);
    ECX_COMPONENT(mouse_follow_comp);
    ECX_COMPONENT(piko::diamonds);

    auto& cam = ecs::get<Camera2D>(Camera2D::Main);
    cam.clearColorEnabled = true;
    cam.clearColor = vec4_color(ARGB(0xFF666666));

    SampleIntegrations::initializePlugins();
}

void DemoApp::preload() {
    basic_application::preload();
}

void DemoApp::onUpdateFrame(float dt) {
    if (currentSample) {
        currentSample->update(dt);
    }
    if (tfFPS) {
        fps_counter_update(&fps_cnt, dt);
        auto fps = (int) fps_cnt.average;
        if (fps != prevFPS) {
            prevFPS = fps;
            set_text_f(tfFPS, "FPS: %d", fps);
        }
    }
}

void DemoApp::onPreRender() {
    preRenderScene3D();
}

void DemoApp::onRenderSceneBefore() {
    renderScene3D(&display.info);

    if (started_ && currentSample) {
        currentSample->draw();
    }
}

void DemoApp::onAppStart() {
//    setup_game(w, game);
    log_debug("Start Demo: prepareInternalResources");
    SampleText::prepareInternalResources();

    SampleBase::samplesContainer = createNode2D(H("sample"));
    append(root, SampleBase::samplesContainer);

    auto prev = createButton("<", [] { scrollSample(-1); });
    auto next = createButton(">", [] { scrollSample(+1); });
    ecs::add<LayoutRect>(prev).enableAlignX(0, 30);
    ecs::add<LayoutRect>(next).enableAlignX(1, -30);

    tfSampleTitle = createNode2D(H("title"));
    addText(tfSampleTitle, "");
    ecs::add<LayoutRect>(tfSampleTitle).enableAlignX(0.5);

    {
        tfFPS = createNode2D(H("fps"));
        addText(tfFPS, "");
        auto& tf = ecs::get<Text2D>(tfFPS);
        tf.format.alignment = vec2(0, 0);
        tf.format.size = 8.0f;
        ecs::add<LayoutRect>(tfFPS)
                .enableAlignX(0.0, 10)
                .enableAlignY(0.0, 10);
        append(root, tfFPS);
    }


    {
        entity_t tf_version = createNode2D(H("version"));
        addText(tf_version, APP_VERSION_NAME "+" APP_VERSION_CODE);
        auto& tf = ecs::get<Text2D>(tf_version);
        tf.format.alignment = vec2(1, 0);
        tf.format.size = 8.0f;
        ecs::add<LayoutRect>(tf_version)
                .enableAlignX(1.0, -10)
                .enableAlignY(0.0, 10);
        append(root, tf_version);
    }

    auto controls = createNode2D(H("controls"));
    append(controls, tfSampleTitle);
    append(controls, prev);
    append(controls, next);
    ecs::add<LayoutRect>(controls).enableAlignY(1, -30);

    append(root, controls);

    log_debug("Start Demo: initSamples");
    initSamples();

    SampleAudio::startMusicTrack();
}

void DemoApp::terminate() {
    delete currentSample;
    currentSample = nullptr;
}

DemoApp::~DemoApp() = default;

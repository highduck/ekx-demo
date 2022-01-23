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
#include <ek/scenex/AudioManager.hpp>
#include <ek/scenex/3d/Scene3D.h>

#ifdef EK_UITEST
#include "screenshots.hpp"
#endif

void ek_app_main() {
    ek_firebase_init();
    ek_app.config.title = "ekx";
    ek_app.config.width = 360;
    ek_app.config.height = 480;
    ek_app.config.need_depth = true;
    ek::run_app<DemoApp>();
}

namespace ek {

Array<std::function<SampleBase*()>> sampleFactory;
int currentSampleIndex = 0;
std::unique_ptr<SampleBase> currentSample = nullptr;
ecs::EntityApi tfSampleTitle;
ecs::EntityApi tfFPS = nullptr;
int prevFPS = 0;
FpsMeter fpsMeter;

void setCurrentSample(int index);

void initSamples() {
    sampleFactory.emplace_back([] { return new SampleSim(); });
    sampleFactory.emplace_back([] { return new SamplePiko(); });
    //sampleFactory.emplace_back([] { return new SampleFlash("test1"); });
    sampleFactory.emplace_back([] { return new SampleFlash("test2"); });
    sampleFactory.emplace_back([] { return new Sample3D(); });
    sampleFactory.emplace_back([] { return new SampleAudio(); });
    sampleFactory.emplace_back([] { return new SampleIntegrations(); });
    sampleFactory.emplace_back([] { return new SampleText(); });
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
    currentSample.reset(sampleFactory[currentSampleIndex]());
    getDrawable<Text2D>(tfSampleTitle).text = currentSample->title;
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

    auto& cam = Camera2D::Main.get<Camera2D>();
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
        fpsMeter.update(dt);
        auto fps = (int) fpsMeter.getAverageFPS();
        if (fps != prevFPS) {
            prevFPS = fps;
            tfFPS.get<Display2D>().get<Text2D>().text = String::format("FPS: %d", fps);
        }
    }
}

void DemoApp::onPreRender() {
    preRenderScene3D();
}

void DemoApp::onRenderSceneBefore() {
    renderScene3D(display);

    if (started_ && currentSample) {
        currentSample->draw();
    }
}

void DemoApp::onAppStart() {
//    setup_game(w, game);
    EK_DEBUG("Start Demo: prepareInternalResources");
    SampleText::prepareInternalResources();

    SampleBase::samplesContainer = createNode2D(H("sample"));
    append(root, SampleBase::samplesContainer);

    auto prev = createButton("<", [] { scrollSample(-1); });
    auto next = createButton(">", [] { scrollSample(+1); });
    prev.assign<LayoutRect>().enableAlignX(0, 30);
    next.assign<LayoutRect>().enableAlignX(1, -30);

    tfSampleTitle = createNode2D(H("title"));
    addText(tfSampleTitle, "");
    tfSampleTitle.assign<LayoutRect>().enableAlignX(0.5);

    {
        tfFPS = createNode2D(H("fps"));
        addText(tfFPS, "");
        tfFPS.get<Display2D>().get<Text2D>().format.alignment = vec2(0, 0);
        tfFPS.assign<LayoutRect>()
                .enableAlignX(0.0, 10)
                .enableAlignY(0.0, 10);
        append(root, tfFPS);
    }

    auto controls = createNode2D(H("controls"));
    append(controls, tfSampleTitle);
    append(controls, prev);
    append(controls, next);
    controls.assign<LayoutRect>().enableAlignY(1, -30);

    append(root, controls);

    EK_DEBUG("Start Demo: initSamples");
    initSamples();

    g_audio->play_music(H("sfx/music1"));
}

DemoApp::~DemoApp() = default;

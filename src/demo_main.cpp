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

#include <ek/math/rand.hpp>
#include <ek/scenex/systems/main_flow.hpp>
#include <ek/scenex/asset2/Asset.hpp>
#include <ek/scenex/SceneFactory.hpp>
#include <ek/scenex/base/Interactive.hpp>
#include <ek/scenex/base/Node.hpp>
#include <ek/scenex/2d/LayoutRect.hpp>
#include <ui/minimal.hpp>
#include <ek/scenex/2d/Camera2D.hpp>
#include <ek/scenex/2d/Display2D.hpp>
#include <ek/debug.hpp>
#include <ek/scenex/AudioManager.hpp>

#ifdef EK_UITEST
#include "screenshots.hpp"
#endif
namespace ek {

std::vector<std::function<SampleBase*()>> sampleFactory;
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

void main() {
    run_app<DemoApp>({"ekx", float2{360, 480}, true});
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

    ECX_COMPONENT(sim::motion_t);
    ECX_COMPONENT(sim::attractor_t);
    ECX_COMPONENT(CameraArcBall);
    ECX_COMPONENT(test_rotation_comp);

    auto& cam = Camera2D::Main.get<Camera2D>();
    cam.clearColorEnabled = true;
    cam.clearColor = float4{0xFF666666_argb};

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
            tfFPS.get<Display2D>().get<Text2D>().text = "FPS: " + std::to_string(fps);
        }
    }
}

void DemoApp::onRenderSceneBefore() {
    if (started_ && currentSample) {
        currentSample->draw();
    }
}

void DemoApp::onAppStart() {
//    setup_game(w, game);
    EK_DEBUG << "Start Demo: prepareInternalResources";
    SampleText::prepareInternalResources();

    SampleBase::samplesContainer = createNode2D("sample");
    append(root, SampleBase::samplesContainer);

    auto prev = createButton("<", [] { scrollSample(-1); });
    auto next = createButton(">", [] { scrollSample(+1); });
    prev.assign<LayoutRect>().enableAlignX(0, 30);
    next.assign<LayoutRect>().enableAlignX(1, -30);

    tfSampleTitle = createNode2D("title");
    addText(tfSampleTitle, "");
    tfSampleTitle.assign<LayoutRect>().enableAlignX(0.5);

    {
        tfFPS = createNode2D("fps");
        addText(tfFPS, "");
        tfFPS.get<Display2D>().get<Text2D>().format.alignment = float2::zero;
        tfFPS.assign<LayoutRect>()
                .enableAlignX(0.0, 10)
                .enableAlignY(0.0, 10);
        append(root, tfFPS);
    }

    auto controls = createNode2D("controls");
    append(controls, tfSampleTitle);
    append(controls, prev);
    append(controls, next);
    controls.assign<LayoutRect>().enableAlignY(1, -30);

    append(root, controls);

    EK_DEBUG << "Start Demo: initSamples";
    initSamples();

    auto& audio = Locator::ref<AudioManager>();
    audio.play_music("sfx/music1");
}

DemoApp::~DemoApp() = default;

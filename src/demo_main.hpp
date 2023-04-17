#pragma once

#include <ek/scenex/app/basic_application.hpp>

class DemoApp : public ::basic_application {
public:
    DemoApp();

    ~DemoApp() override;

    void initialize() override;

    void preload() override;

    void terminate() override;

protected:
    void onUpdateFrame(float dt) override;

    void onPreRender() override;

    void onRenderSceneBefore() override;

    void onAppStart() override;
};
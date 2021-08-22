#pragma once

#include "sample_base.hpp"
#include <ek/scenex/AudioManager.hpp>
#include <ek/util/ServiceLocator.hpp>
#include <ek/scenex/2d/Display2D.hpp>
#include <ek/scenex/2d/Transform2D.hpp>
#include <ek/scenex/base/Node.hpp>
#include "ui/minimal.hpp"

namespace ek {

class SampleAudio : public SampleBase {
    inline static uint32_t trackIndex = 0;
public:
    ~SampleAudio() override = default;

    SampleAudio() : SampleBase() {
        title = "AUDIO";

        float spaceY = 60.0f;
        float2 pos{360.0f / 2, 50.0f};
        auto btn = createButton("NEXT TRACK", [] {
            ++trackIndex;
            startMusicTrack();
        });
        setPosition(btn, pos);
        getDrawable<Text2D>(btn).rect.set(-100, -25, 200, 50);
        append(container, btn);
        pos.y += spaceY;
    }

    static void startMusicTrack() {
        auto& audio = Locator::ref<AudioManager>();
        const char* music = ((trackIndex & 1) == 0) ? "sfx/music1" : "sfx/music2";
        audio.play_music(music);
    }
};

}
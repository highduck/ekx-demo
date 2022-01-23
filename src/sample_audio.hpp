#pragma once

#include "sample_base.hpp"
#include <ek/scenex/AudioManager.hpp>

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
        vec2_t pos = vec2(360.0f / 2, 50.0f);
        auto btn = createButton("NEXT TRACK", [] {
            ++trackIndex;
            startMusicTrack();
        });
        setPosition(btn, pos);
        getDrawable<Text2D>(btn).rect = {{-100, -25, 200, 50}};
        append(container, btn);
        pos.y += spaceY;

        btn = createButton("VIBRATE 10 MS", [] {
            auph_vibrate(10);
        });
        setPosition(btn, pos);
        getDrawable<Text2D>(btn).rect = {{-100, -25, 200, 50}};
        append(container, btn);
        pos.y += spaceY;

        btn = createButton("VIBRATE 100 MS", [] {
            auph_vibrate(100);
        });
        setPosition(btn, pos);
        getDrawable<Text2D>(btn).rect = {{-100, -25, 200, 50}};
        append(container, btn);
        pos.y += spaceY;
    }

    static void startMusicTrack() {
        string_hash_t music = ((trackIndex & 1) == 0) ? H("sfx/music1") : H("sfx/music2");
        g_audio->play_music(music);
    }
};

}
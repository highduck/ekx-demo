#pragma once

#include "sample_base.hpp"
#include <ekx/app/audio_manager.h>

#include <ek/scenex/2d/Display2D.hpp>
#include <ek/scenex/2d/Transform2D.hpp>
#include <ek/scenex/base/Node.hpp>
#include "ui/minimal.hpp"
#include <ek/local_res.h>

#include "modplay/pocketmod.h"

typedef struct user_stream {
    void* userdata;
    void* callback;
    uint64_t cursor;
    float prev[24];
} user_stream;

void play_mod_stream(void* userdata, float* samples, uint32_t count) {
    pocketmod_context* ctx = (pocketmod_context*) userdata;
    uint32_t i = 0;
    uint32_t size = count * sizeof(float) * 2;
    while (i < size) {
        uint32_t rendered_bytes = pocketmod_render(ctx, (uint8_t*) samples + i, size - i);
        i += rendered_bytes;
    }
}

namespace ek {

class SampleAudio : public SampleBase {
    inline static uint32_t trackIndex = 0;
    inline static pocketmod_context ctx;
    inline static ek_local_res slr;
    inline static auph_buffer buf;
    inline static auph_voice vc;
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
        set_position(btn, pos);
        ecs::get<Text2D>(btn).rect = {{-100, -25, 200, 50}};
        append(container, btn);
        pos.y += spaceY;

        btn = createButton("VIBRATE 10 MS", [] {
            auph_vibrate(10);
        });
        set_position(btn, pos);
        ecs::get<Text2D>(btn).rect = {{-100, -25, 200, 50}};
        append(container, btn);
        pos.y += spaceY;

        btn = createButton("VIBRATE 100 MS", [] {
            auph_vibrate(100);
        });
        set_position(btn, pos);
        ecs::get<Text2D>(btn).rect = {{-100, -25, 200, 50}};
        append(container, btn);
        pos.y += spaceY;

        btn = createButton("X0.5", [] {
            if(vc.id) {
                auph_set_rate(vc, 0.5f);
            }
        });
        set_position(btn, pos);
        ecs::get<Text2D>(btn).rect = {{-100, -25, 50, 50}};
        append(container, btn);

        btn = createButton("X1", [] {
            if(vc.id) {
                auph_set_rate(vc, 1.0f);
            }
        });
        set_position(btn, pos);
        ecs::get<Text2D>(btn).rect = {{-50, -25, 50, 50}};
        append(container, btn);

        btn = createButton("X2", [] {
            if(vc.id) {
                auph_set_rate(vc, 2.0f);
            }
        });
        set_position(btn, pos);
        ecs::get<Text2D>(btn).rect = {{0, -25, 50, 50}};
        append(container, btn);

        pos.y += spaceY;

        btn = createButton("RESTART", [] {
            if(vc.id) {
                ctx.pattern = 0;
                ctx.sample = 0;
                ctx.tick = 0;
                ctx.line = 0;
            }
        });
        set_position(btn, pos);
        ecs::get<Text2D>(btn).rect = {{-50, -25, 200, 50}};
        append(container, btn);

        pos.y += spaceY;

    }

    void update(float) override {

    }

    static void startMusicTrack() {
        string_hash_t music = ((trackIndex & 1) == 0) ? H("sfx/music1") : H("sfx/music2");
//        play_music(music);

        static const char* mods_path[] = {
                "assets/mods/virgill_-_cherry_coke.mod",
                "assets/mods/bananasplit.mod",
                "assets/mods/chill.mod",
                "assets/mods/elysium.mod",
                "assets/mods/king.mod",
                "assets/mods/nemesis.mod",
                "assets/mods/overture.mod",
                "assets/mods/spacedeb.mod",
                "assets/mods/stardstm.mod",
                "assets/mods/sundance.mod",
                "assets/mods/sundown.mod",
                "assets/mods/supernova.mod",
        };


        if (slr.buffer) {
            auph_stop(vc.id);
            auph_unload(buf);
            ek_local_res_close(&slr);
            slr = {};
        }
        const char* track_path = mods_path[trackIndex];
        trackIndex = (trackIndex + 1) % (sizeof(mods_path) / sizeof(mods_path[0]));
        log_info("Play %s", track_path);
        ek_local_res_load(track_path,
                          [](ek_local_res* lr) {
                              slr = *lr;
                              if (ek_local_res_success(lr)) {

                                  if (pocketmod_init(&ctx, lr->buffer, lr->length,
                                                     auph_get(AUPH_MIXER, AUPH_PARAM_SAMPLE_RATE))) {
                                      auto* stream = (user_stream*) malloc(sizeof(user_stream));
                                      stream->userdata = &ctx;
                                      stream->callback = (void*) play_mod_stream;
                                      buf = auph_load_data(stream, 0, 16);

                                      vc = auph_play_f(buf, 1, 0, 0.5f, true, false, AUPH_BUS_MUSIC);
                                  } else {
                                      log_error("can't read MOD file");
                                  }
                              }
                              //
                          }, nullptr);
    }
};

}
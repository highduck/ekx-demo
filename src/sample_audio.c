#include "sample_audio.h"
#include <ekx/app/audio_manager.h>

#include "ui/minimal_ui.h"
#include <ek/local_res.h>
#include <ek/log.h>
#include <ek/scenex/2d/text2d.h>
#include <ek/scenex/2d/transform2d.h>
#include <ek/scenex/base/node.h>

#include <pocketmod/pocketmod.h>

uint32_t track_index;
pocketmod_context ctx;
ek_local_res slr;
auph_buffer buf;
auph_voice vc;

static void play_mod_stream(void* userdata, float* samples, uint32_t count) {
    pocketmod_context* ctx = (pocketmod_context*)userdata;
    uint32_t i = 0;
    uint32_t size = count * sizeof(float) * 2;
    while (i < size) {
        uint32_t rendered_bytes = pocketmod_render(ctx, (uint8_t*)samples + i, size - i);
        i += rendered_bytes;
    }
}

static void on_music_track_loaded(ek_local_res* lr) {
    slr = *lr;
    if (ek_local_res_success(lr)) {
        const int sample_rate = auph_get(AUPH_MIXER, AUPH_PARAM_SAMPLE_RATE);
        log_info("init pocketmod: sample rate = %d, buffer length = %d, bytes = %p",
                 sample_rate, (int)lr->length, lr->buffer);
        if (pocketmod_init(&ctx, lr->buffer, (int)lr->length, sample_rate)) {
            buf = auph_load_callback(play_mod_stream, &ctx);
            vc = auph_play_f(buf, 1, 0, 0.5f, true, false, AUPH_BUS_MUSIC);
        } else {
            log_error("can't read MOD file");
        }
    }
}

void play_track_music(void) {
    //        string_hash_t music = ((trackIndex & 1) == 0) ? H("sfx/music1") : H("sfx/music2");
    //        play_music(music);

    const char* mods_path[12] = {
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
        slr = (ek_local_res){0};
    }
    track_index = track_index % (sizeof(mods_path) / sizeof(mods_path[0]));
    const char* track_path = mods_path[track_index];
    log_info("Play # %d : %s", track_index, track_path);
    ek_local_res_load(track_path, on_music_track_loaded, NULL);
}

static void on_next_track(const node_event_t* event) {
    UNUSED(event);
    ++track_index;
    play_track_music();
}

static void on_vibrate_10(const node_event_t* event) {
    UNUSED(event);
    auph_vibrate(10);
}

static void on_vibrate_100(const node_event_t* event) {
    UNUSED(event);
    auph_vibrate(100);
}

static void on_rate_half(const node_event_t* event) {
    UNUSED(event);
    if (vc.id) {
        auph_set_rate(vc, 0.5f);
    }
}

static void on_rate_normal(const node_event_t* event) {
    UNUSED(event);
    if (vc.id) {
        auph_set_rate(vc, 1.0f);
    }
}

static void on_rate_double(const node_event_t* event) {
    UNUSED(event);
    if (vc.id) {
        auph_set_rate(vc, 2.0f);
    }
}

static void on_restart_player(const node_event_t* event) {
    UNUSED(event);
    if (vc.id) {
        ctx.pattern = 0;
        ctx.sample = 0;
        ctx.tick = 0;
        ctx.line = 0;
    }
}

void start_sample_audio(entity_t container) {
    const float space_y = 60.0f;
    vec2_t pos = vec2(360.0f / 2, 50.0f);
    entity_t btn = create_button("NEXT TRACK", on_next_track);
    set_position(btn, pos);
    get_text2d(btn)->rect = rect(-100, -25, 200, 50);
    append(container, btn);
    pos.y += space_y;

    btn = create_button("VIBRATE 10 MS", on_vibrate_10);
    set_position(btn, pos);
    get_text2d(btn)->rect = rect(-100, -25, 200, 50);
    append(container, btn);
    pos.y += space_y;

    btn = create_button("VIBRATE 100 MS", on_vibrate_100);
    set_position(btn, pos);
    get_text2d(btn)->rect = rect(-100, -25, 200, 50);
    append(container, btn);
    pos.y += space_y;

    btn = create_button("X0.5", on_rate_half);
    set_position(btn, pos);
    get_text2d(btn)->rect = rect(-100, -25, 50, 50);
    append(container, btn);

    btn = create_button("X1", on_rate_normal);
    set_position(btn, pos);
    get_text2d(btn)->rect = rect(-50, -25, 50, 50);
    append(container, btn);

    btn = create_button("X2", on_rate_double);
    set_position(btn, pos);
    get_text2d(btn)->rect = rect(0, -25, 50, 50);
    append(container, btn);

    pos.y += space_y;

    btn = create_button("RESTART", on_restart_player);
    set_position(btn, pos);
    get_text2d(btn)->rect = rect(-50, -25, 200, 50);
    append(container, btn);

    pos.y += space_y;
}

sample_t sample_audio(void) {
    sample_t s = INIT_ZERO;
    s.start = start_sample_audio;
    return s;
}

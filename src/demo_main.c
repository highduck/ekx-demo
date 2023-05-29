#include "sample_text.h"
#include "sample_flash.h"
#include "render3d/scene3d.h"
#include "3d/sample_3d.h"
#include "piko/sample_piko.h"
#include "sim/sample_sim.h"
#include "sample_integrations.h"
#include "sample_audio.h"
#include "sim/motion.h"
#include "sim/follow.h"
#include "3d/camera_arcball.h"
#include "piko/examples.h"
#include "config/build_info.h"
#include "ui/minimal_ui.h"

#include <ek/firebase.h>
#include <ek/scenex/app/base_game.h>
#include <ek/scenex/systems/main_flow.h>
#include <ek/scenex/scene_factory.h>
#include <ek/scenex/base/node.h>
#include <ek/scenex/2d/layout_rect.h>
#include <ek/scenex/2d/camera2d.h>
#include <ek/scenex/2d/text2d.h>
#include <ek/log.h>
#include <sce/util/time_layers.h>
#include <appbox/appbox.h>
#include <ekx/app/profiler.h>

#ifdef EK_UITEST
#include "screenshots.h"
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
    ek_app.config.width = WIDTH;
    ek_app.config.height = HEIGHT;
    ek_app.config.need_depth = true;
    game_app_state.delegate.pre_load = on_pre_load;
    game_app_state.delegate.start = on_start;
    game_app_state.delegate.update = on_update;
    game_app_state.delegate.pre_render = on_pre_render;
    game_app_state.delegate.pre_render_scene = on_pre_render_scene;
    game_app_state.delegate.terminate = on_terminate;
    run_app();
}

enum{
    SAMPLES_NUM = 8,
};

sample_t samples[SAMPLES_NUM];

int current_sample_index;
sample_t* current_sample;
entity_t tf_sample_title;
entity_t tf_fps;
int prev_fps;
fps_counter fps_cnt;

void activate_sample(int index);

void init_samples() {
    samples[0] = sample_piko();
    samples[1] = sample_sim();
    samples[2] = sample_flash1();
    samples[3] = sample_flash2();
    samples[4] = sample3d();
    samples[5] = sample_audio();
    samples[6] = sample_integrations();
    samples[7] = sample_text();
    activate_sample(0);
}

void activate_sample(int index) {
    current_sample_index = index;
    if (current_sample_index >= SAMPLES_NUM) {
        current_sample_index = 0;
    }
    if (current_sample_index < 0) {
        current_sample_index = SAMPLES_NUM - 1;
    }
    stop_sample(current_sample);
    current_sample = &samples[current_sample_index];
    set_text(tf_sample_title, current_sample->title);
    start_sample(current_sample);
}

void scroll_sample(int delta) {
    activate_sample(current_sample_index + delta);
}

static void on_pre_render() {
    scene3d_prerender();
    pre_render_diamonds();
}

static void on_pre_render_scene(void) {
    scene3d_render(&game_app_state.display.info);
    if (game_app_state.started && current_sample && current_sample->draw) {
        current_sample->draw();
    }
}

static void on_pre_load(void) {
#ifdef EK_UITEST
    uitest("screenshots", do_game_screenshots);
#endif

    setup_scene3d();

    ECX_TYPE(motion_t, 32);
    ECX_TYPE(attractor_t, 32);
    ECX_TYPE(camera_arc_ball_t, 2);
    ECX_TYPE_0(test_rotation3d_t, 32);
    setup_follow();

    camera2d_t* cam = get_camera2d(main_camera);
    cam->clearColorEnabled = true;
    cam->clearColor = vec4_color(ARGB(0xFF666666));

    sample_plugins_setup();
}

void on_update(void) {
    const float dt = g_time_dt;
    if (current_sample && current_sample->update) {
        current_sample->update();
    }
    if (tf_fps.id) {
        fps_counter_update(&fps_cnt, dt);
        int fps = (int) fps_cnt.average;
        if (fps != prev_fps) {
            prev_fps = fps;
            set_text_f(tf_fps, "FPS: %d", fps);
        }
    }
}

static void on_select_prev(const node_event_t* event) {
    UNUSED(event);
    scroll_sample(-1);
}

static void on_select_next(const node_event_t* event) {
    UNUSED(event);
    scroll_sample(1);
}

static void on_start(void) {
    appbox_on_game_start();

    const entity_t root = game_app_state.root;
    log_debug("Start Demo: prepareInternalResources");
    preload_sample_text();

    samples_container = create_node2d(H("sample"));
    append(root, samples_container);

    entity_t prev = create_button("<", on_select_prev);
    entity_t next = create_button(">", on_select_next);
    LayoutRect_enableAlignX(add_layout_rect(prev), 0, 30);
    LayoutRect_enableAlignX(add_layout_rect(next), 1, -30);

    tf_sample_title = create_node2d(H("title"));
    add_text(tf_sample_title, "");
    LayoutRect_enableAlignX(add_layout_rect(tf_sample_title), 0.5, 0);

    {
        tf_fps = create_node2d(H("fps"));
        add_text(tf_fps, "");
        text2d_t* tf = get_text2d(tf_fps);
        tf->format.alignment = vec2(0, 0);
        tf->format.size = 8.0f;
        layout_rect_t* ll = add_layout_rect(tf_fps);
        LayoutRect_enableAlignX(ll, 0.0, 10);
        LayoutRect_enableAlignY(ll, 0.0, 10);
        append(root, tf_fps);
    }


    {
        entity_t tf_version = create_node2d(H("version"));
        add_text(tf_version, APP_VERSION_NAME "+" APP_VERSION_CODE);
        text2d_t* tf = get_text2d(tf_version);
        tf->format.alignment = vec2(1, 0);
        tf->format.size = 8.0f;
        layout_rect_t* ll = add_layout_rect(tf_version);
        LayoutRect_enableAlignX(ll, 1.0, -10);
        LayoutRect_enableAlignY(ll, 0.0, 10);
        append(root, tf_version);
    }

    entity_t controls = create_node2d(H("controls"));
    append(controls, tf_sample_title);
    append(controls, prev);
    append(controls, next);
    LayoutRect_enableAlignY(add_layout_rect(controls), 1, -30);

    append(root, controls);

    log_debug("Start Demo: initSamples");
    init_samples();

    play_track_music();
}

static void on_terminate(void) {
    stop_sample(current_sample);
    current_sample = NULL;
}

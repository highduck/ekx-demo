#include "sample_integrations.h"
#include <appbox/ads.h>
#include <ek/admob.h>
#include <ek/game_services.h>
#include <ek/scenex/2d/text2d.h>
#include <ek/scenex/2d/transform2d.h>
#include <ek/scenex/base/node.h>
#include <ui/minimal_ui.h>

static void on_post_score(const node_event_t* event) {
    UNUSED(event);
    static int best_score = 1;
    ek_leaderboard_submit("CgkIpvfh798IEAIQAA", ++best_score);
}

static void on_leaderboard(const node_event_t* event) {
    UNUSED(event);
    ek_leaderboard_show("CgkIpvfh798IEAIQAA");
}

static void on_achievements(const node_event_t* event) {
    UNUSED(event);
    ek_achievement_show();
}

static void on_interstitial_ad(const node_event_t* event) {
    UNUSED(event);
    ads_game_over(NULL);
}

static void on_video_ad(const node_event_t* event) {
    UNUSED(event);
    ads_show_reward_video(NULL);
}

static void on_remove_ads(const node_event_t* event) {
    UNUSED(event);
    ads_purchase_remove_ads();
}

static void on_crash_me(const node_event_t* event) {
    UNUSED(event);
    // simple null-pointer abort
    ((char*)0)[0] = 1;
}

void start_sample_integrations(entity_t container) {
    // TODO: show all leaderboards, achievements, ads
    const float space_y = 40.0f;
    const rect_t default_rect = rect(-100, -15, 200, 30);
    vec2_t pos = vec2(360.0f / 2, 50.0f);

    entity_t btn = create_button("POST SCORE", on_post_score);
    set_position(btn, pos);
    get_text2d(btn)->rect = default_rect;
    append(container, btn);
    pos.y += space_y;

    btn = create_button("LEADERBOARD", on_leaderboard);
    set_position(btn, pos);
    get_text2d(btn)->rect = default_rect;
    append(container, btn);
    pos.y += space_y;

    btn = create_button("ACHIEVEMENTS", on_achievements);
    set_position(btn, pos);
    get_text2d(btn)->rect = default_rect;
    append(container, btn);
    pos.y += space_y;

    pos.y += 10.0f;
    btn = create_button("INTERSTITIAL AD", on_interstitial_ad);
    set_position(btn, pos);
    get_text2d(btn)->rect = default_rect;
    append(container, btn);
    pos.y += space_y;

    btn = create_button("VIDEO AD", on_video_ad);
    set_position(btn, pos);
    get_text2d(btn)->rect = default_rect;
    append(container, btn);
    pos.y += space_y;

    btn = create_button("REMOVE ADS", on_remove_ads);
    set_position(btn, pos);
    get_text2d(btn)->rect = default_rect;
    append(container, btn);
    pos.y += space_y;

    btn = create_button("CRASH ME", on_crash_me);
    set_position(btn, pos);
    get_text2d(btn)->rect = default_rect;
    append(container, btn);
    pos.y += space_y;
}

void sample_plugins_setup() {
    const char* billing_key = "";
    ek_admob_config admob_config = INIT_ZERO;
    admob_config.child_directed = EK_ADMOB_CHILD_DIRECTED_UNSPECIFIED;

    ads_premium_config ads_premium_conf;
    ads_premium_conf.key0 = "_ads_removed_purchase_cache";
    ads_premium_conf.val0 = 12345;
    ads_premium_conf.key1 = "_ads_removed_key";
    ads_premium_conf.val1 = 98765;
    ads_premium_conf.sku_remove_ads = "remove_ads";
#if EK_ANDROID
    admob_config.banner = "ca-app-pub-3931267664278058/7752333837";
    admob_config.inters = "ca-app-pub-3931267664278058/5126170492";
    admob_config.video = "ca-app-pub-3931267664278058/1733720395";
#elif EK_IOS
    admob_config.banner = "ca-app-pub-3931267664278058/6010811099";
    admob_config.inters = "ca-app-pub-3931267664278058/4697729428";
    admob_config.video = "ca-app-pub-3931267664278058/5819239403";
#endif

    billing_setup(billing_key);
    ek_admob_init(admob_config);
    ads_init(ads_premium_conf);
    ek_game_services_init();
}

sample_t sample_integrations(void) {
    sample_t s = INIT_ZERO;
    s.title = "SERVICE";
    s.start = start_sample_integrations;
    return s;
}

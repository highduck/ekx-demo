#include <ui/minimal_ui.h>
#include <ek/game_services.h>
#include <ek/scenex/2d/text2d.h>
#include <ek/scenex/2d/transform2d.h>
#include <ek/scenex/base/node.h>
#include <ek/admob.h>
#include <appbox/ads.h>
#include "sample_integrations.hpp"

namespace ek {

SampleIntegrations::SampleIntegrations() :
        SampleBase() {
    title = "SERVICE";

    // TODO: show all leaderboards, achievements, ads

    float spaceY = 40.0f;
    rect_t default_rect = rect(-100, -15, 200, 30);
    vec2_t pos = vec2(360.0f / 2, 50.0f);

    auto btn = create_button("POST SCORE", +[](const node_event_t*) {
        static int bestScore = 1;
        ek_leaderboard_submit("CgkIpvfh798IEAIQAA", ++bestScore);
    });
    set_position(btn, pos);
    get_text2d(btn)->rect = default_rect;
    append(container, btn);
    pos.y += spaceY;

    btn = create_button("LEADERBOARD", +[](const node_event_t*) {
        ek_leaderboard_show("CgkIpvfh798IEAIQAA");
    });
    set_position(btn, pos);
    get_text2d(btn)->rect = default_rect;
    append(container, btn);
    pos.y += spaceY;

    btn = create_button("ACHIEVEMENTS", +[](const node_event_t*) {
        ek_achievement_show();
    });
    set_position(btn, pos);
    get_text2d(btn)->rect = default_rect;
    append(container, btn);
    pos.y += spaceY;

    pos.y += 10.0f;
    btn = create_button("INTERSTITIAL AD", +[](const node_event_t*) {
        ads_game_over(+[]() {});
    });
    set_position(btn, pos);
    get_text2d(btn)->rect = default_rect;
    append(container, btn);
    pos.y += spaceY;

    btn = create_button("VIDEO AD", +[](const node_event_t*) {
        ads_show_reward_video(+[](bool) {});
    });
    set_position(btn, pos);
    get_text2d(btn)->rect = default_rect;
    append(container, btn);
    pos.y += spaceY;

    btn = create_button("REMOVE ADS", +[](const node_event_t*) {
        ads_purchase_remove_ads();
    });
    set_position(btn, pos);
    get_text2d(btn)->rect = default_rect;
    append(container, btn);
    pos.y += spaceY;

    btn = create_button("CRASH ME", +[](const node_event_t*) {
        // simple null-pointer abort
        ((SampleIntegrations*) nullptr)->title = "";
    });

    set_position(btn, pos);
    get_text2d(btn)->rect = default_rect;
    append(container, btn);
    pos.y += spaceY;
}

}

void sample_plugins_setup() {
    const char* billing_key = "";
    ek_admob_config admob_config = {};
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

#include <ui/minimal.hpp>
#include <ek/game_services.h>
#include <ek/scenex/2d/Display2D.hpp>
#include <ek/scenex/2d/Transform2D.hpp>
#include <ek/scenex/base/node.h>
#include <ek/admob.h>
#include <appbox/Ads.hpp>
#include "sample_integrations.hpp"

namespace ek {

SampleIntegrations::SampleIntegrations() :
        SampleBase() {
    title = "SERVICE";

    // TODO: show all leaderboards, achievements, ads

    float spaceY = 40.0f;
    rect_t default_rect = rect(-100, -15, 200, 30);
    vec2_t pos = vec2(360.0f / 2, 50.0f);

    auto btn = createButton("POST SCORE", [] {
        static int bestScore = 1;
        ek_leaderboard_submit("CgkIpvfh798IEAIQAA", ++bestScore);
    });
    set_position(btn, pos);
    ecs::get<Text2D>(btn).rect = default_rect;
    append(container, btn);
    pos.y += spaceY;

    btn = createButton("LEADERBOARD", [] {
        ek_leaderboard_show("CgkIpvfh798IEAIQAA");
    });
    set_position(btn, pos);
    ecs::get<Text2D>(btn).rect = default_rect;
    append(container, btn);
    pos.y += spaceY;

    btn = createButton("ACHIEVEMENTS", [] {
        ek_achievement_show();
    });
    set_position(btn, pos);
    ecs::get<Text2D>(btn).rect = default_rect;
    append(container, btn);
    pos.y += spaceY;

    pos.y += 10.0f;
    btn = createButton("INTERSTITIAL AD", [] {
        g_ads->gameOver([] {
            // TODO:
        });
    });
    set_position(btn, pos);
    ecs::get<Text2D>(btn).rect = default_rect;
    append(container, btn);
    pos.y += spaceY;

    btn = createButton("VIDEO AD", [] {
        g_ads->showRewardVideo([](bool rewarded) {
            (void)rewarded;
            // TODO:
        });
    });
    set_position(btn, pos);
    ecs::get<Text2D>(btn).rect =default_rect;
    append(container, btn);
    pos.y += spaceY;

    btn = createButton("REMOVE ADS", [] {
        g_ads->purchaseRemoveAds();
    });
    set_position(btn, pos);
    ecs::get<Text2D>(btn).rect = default_rect;
    append(container, btn);
    pos.y += spaceY;

    btn = createButton("CRASH ME", [] {
        // simple null-pointer abort
        ((SampleIntegrations*)nullptr)->title = "";
    });

    set_position(btn, pos);
    ecs::get<Text2D>(btn).rect = default_rect;
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

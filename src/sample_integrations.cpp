#include <ui/minimal.hpp>
#include <ek/game_services.h>
#include <ek/scenex/2d/Display2D.hpp>
#include <ek/scenex/2d/Transform2D.hpp>
#include <ek/scenex/base/node.h>
#include <ek/admob.h>
#include <appbox/Ads.hpp>
#include <ek/firebase.h>
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

void SampleIntegrations::initializePlugins() {
    const char* billingKey = "";
    ek_admob_config adMobConfig{};
    adMobConfig.child_directed = EK_ADMOB_CHILD_DIRECTED_UNSPECIFIED;

    ads_premium_config adHelperConfig{};
#if EK_ANDROID
    adMobConfig.banner = "ca-app-pub-3931267664278058/7752333837";
    adMobConfig.inters = "ca-app-pub-3931267664278058/5126170492";
    adMobConfig.video = "ca-app-pub-3931267664278058/1733720395";
    adHelperConfig.sku_remove_ads = "remove_ads";
#elif EK_IOS
    adMobConfig.banner = "ca-app-pub-3931267664278058/6010811099";
    adMobConfig.inters = "ca-app-pub-3931267664278058/4697729428";
    adMobConfig.video = "ca-app-pub-3931267664278058/5819239403";
    adHelperConfig.sku_remove_ads = "remove_ads";
#endif

    billing::initialize(billingKey);
    ek_admob_init(adMobConfig);

    adHelperConfig.key0 = "_ads_removed_purchase_cache";
    adHelperConfig.val0 = 12345;
    adHelperConfig.key1 = "_ads_removed_key";
    adHelperConfig.val1 = 98765;

    ads_init(adHelperConfig);

    ek_game_services_init();
}

}

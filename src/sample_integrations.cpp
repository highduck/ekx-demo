#include <ui/minimal.hpp>
#include <ek/game_services.h>
#include <ek/scenex/2d/Display2D.hpp>
#include <ek/scenex/2d/Transform2D.hpp>
#include <ek/scenex/base/Node.hpp>
#include <ek/admob.h>
#include <appbox/Ads.hpp>
#include "sample_integrations.hpp"
#include <ek/util/ServiceLocator.hpp>

namespace ek {

SampleIntegrations::SampleIntegrations() :
        SampleBase() {
    title = "SERVICE";

    // TODO: show all leaderboards, achievements, ads

    float spaceY = 60.0f;
    vec2_t pos = vec2(360.0f / 2, 50.0f);
    auto btn = createButton("POST SCORE", [] {
        static int bestScore = 1;
        ek_leaderboard_submit("CgkIpvfh798IEAIQAA", ++bestScore);
    });
    setPosition(btn, pos);
    getDrawable<Text2D>(btn).rect = {{-100, -25, 200, 50}};
    append(container, btn);
    pos.y += spaceY;


    btn = createButton("LEADERBOARD", [] {
        ek_leaderboard_show("CgkIpvfh798IEAIQAA");
    });
    setPosition(btn, pos);
    getDrawable<Text2D>(btn).rect = {{-100, -25, 200, 50}};
    append(container, btn);
    pos.y += spaceY;

    btn = createButton("ACHIEVEMENTS", [] {
        ek_achievement_show();
    });
    setPosition(btn, pos);
    getDrawable<Text2D>(btn).rect = {{-100, -25, 200, 50}};
    append(container, btn);
    pos.y += spaceY;

    pos.y += 10.0f;
    btn = createButton("INTERSTITIAL AD", [] {
        Locator::ref<Ads>().gameOver([] {
            // TODO:
        });
    });
    setPosition(btn, pos);
    getDrawable<Text2D>(btn).rect = {{-100, -25, 200, 50}};
    append(container, btn);
    pos.y += spaceY;

    btn = createButton("VIDEO AD", [] {
        Locator::ref<Ads>().showRewardVideo([](bool rewarded) {
            (void)rewarded;
            // TODO:
        });
    });
    setPosition(btn, pos);
    getDrawable<Text2D>(btn).rect = {{-100, -25, 200, 50}};
    append(container, btn);
    pos.y += spaceY;

    btn = createButton("REMOVE ADS", [] {
        Locator::ref<Ads>().purchaseRemoveAds();
    });
    setPosition(btn, pos);
    getDrawable<Text2D>(btn).rect = {{-100, -25, 200, 50}};
    append(container, btn);
    pos.y += spaceY;

    btn = createButton("CRASH ME", [] {
        SampleIntegrations* s = Locator::get<SampleIntegrations>();
        // for sure we have not register this sample globally ;)
        s->title = "";
    });

    setPosition(btn, pos);
    getDrawable<Text2D>(btn).rect = {{-100, -25, 200, 50}};
    append(container, btn);
    pos.y += spaceY;
}

void SampleIntegrations::initializePlugins() {
    const char* billingKey = "";
    ek_admob_config adMobConfig{};
    adMobConfig.child_directed = EK_ADMOB_CHILD_DIRECTED_UNSPECIFIED;

    Ads::Config adHelperConfig{};
#if EK_ANDROID
    adMobConfig.banner = "ca-app-pub-3931267664278058/7752333837";
    adMobConfig.inters = "ca-app-pub-3931267664278058/5126170492";
    adMobConfig.video = "ca-app-pub-3931267664278058/1733720395";
    adHelperConfig.skuRemoveAds = "remove_ads";
#elif EK_IOS
    adMobConfig.banner = "ca-app-pub-3931267664278058/6010811099";
    adMobConfig.inters = "ca-app-pub-3931267664278058/4697729428";
    adMobConfig.video = "ca-app-pub-3931267664278058/5819239403";
    adHelperConfig.skuRemoveAds = "remove_ads";
#endif

    billing::initialize(billingKey);
    ek_admob_init(adMobConfig);

    adHelperConfig.key0 = "_ads_removed_purchase_cache";
    adHelperConfig.val0 = 12345;
    adHelperConfig.key1 = "_ads_removed_key";
    adHelperConfig.val1 = 98765;

    Locator::create<Ads>(adHelperConfig);

    ek_game_services_init();
}

}
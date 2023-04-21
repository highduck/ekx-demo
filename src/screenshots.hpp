#pragma once

#include <ek/app.h>

#include <ek/time.h>
#include <ekx/app/uitest.h>
#include <ekx/app/localization.h>
#include <ek/assert.h>
#include <ek/scenex/2d/button.h>
#include <ek/scenex/base/interactive.h>
#include <appbox/Ads.hpp>

#define TIMEOUT_BEGIN  ek_set_timeout((ek_timer_callback){+[](void*)->void{
#define TIMEOUT_END(seconds)  },0,0},(seconds));

void do_game_screenshots(void) {
    g_ads->cheat_RemoveAds();
    TIMEOUT_BEGIN
        uitest_screenshot("trails");
        uitest_click(H("controls"), H(">"));
        TIMEOUT_BEGIN
            uitest_screenshot("pico");
            uitest_click(H("controls"), H(">"));
            TIMEOUT_BEGIN
                uitest_screenshot("flash");
                uitest_click(H("controls"), H(">"));
                TIMEOUT_BEGIN
                    uitest_screenshot("3d");
                    uitest_click(H("controls"), H(">"));
                    TIMEOUT_BEGIN
                        uitest_screenshot("audio");
                        uitest_click(H("controls"), H(">"));
                        TIMEOUT_BEGIN
                            uitest_screenshot("text");
                            uitest_done();
                        TIMEOUT_END(2.0)
                    TIMEOUT_END(2.0)
                TIMEOUT_END(2.0)
            TIMEOUT_END(2.0)
        TIMEOUT_END(2.0)
    TIMEOUT_END(3.0)
}


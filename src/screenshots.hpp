#pragma once

#include <ek/app.h>

#include <ek/scenex/app/uitest.hpp>
#include <ekx/app/localization.h>
#include <ek/assert.h>
#include <ek/scenex/2d/button.h>
#include <ek/scenex/base/interactive.h>
#include <appbox/Ads.hpp>

namespace ek::uitest {

void runScreenshotScript() {
    ads_instance->cheat_RemoveAds();
    setTimeout([] {
        screenshot("trails");
        click({"controls", ">"});
        setTimeout([] {
            screenshot("pico");
            click({"controls", ">"});
            setTimeout([] {
                screenshot("flash");
                click({"controls", ">"});
                setTimeout([] {
                    screenshot("3d");
                    click({"controls", ">"});
                    setTimeout([] {
                        screenshot("audio");
                        click({"controls", ">"});
                        setTimeout([] {
                            screenshot("text");
                            done();
                        }, 2.0);
                    }, 2.0);
                }, 2.0);
            }, 2.0);
        }, 2.0);
    }, 3.0);
}

}

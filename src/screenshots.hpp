#pragma once

#include <ek/app.h>
#include <ek/util/ServiceLocator.hpp>
#include <ek/scenex/app/uitest.hpp>
#include <ek/scenex/Localization.hpp>
#include <ek/assert.h>
#include <ek/scenex/2d/Button.hpp>
#include <ek/scenex/base/Interactive.hpp>
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

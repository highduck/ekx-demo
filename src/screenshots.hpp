#pragma once

#include <ek/app/app.hpp>
#include <ek/util/locator.hpp>
#include <ek/scenex/app/uitest.hpp>
#include <ek/Localization.hpp>
#include <ek/assert.hpp>
#include <ek/scenex/2d/Button.hpp>
#include <ek/scenex/base/Interactive.hpp>
#include <appbox/Ads.hpp>

namespace ek::uitest {

void runScreenshotScript() {
    resolve<Ads>().cheat_RemoveAds();
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

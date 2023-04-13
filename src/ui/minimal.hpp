#pragma once

#include <ecx/ecx.h>
#include <functional>
#include <ek/scenex/2d/text2d.h>

namespace ek {

text2d_t* addText(entity_t e, const char* text);

entity_t createButton(const char* label, const std::function<void()>& fn);

}


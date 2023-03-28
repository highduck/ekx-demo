#pragma once

#include <ecx/ecx.hpp>
#include <functional>

namespace ek {

struct Text2D;

Text2D& addText(entity_t e, const char* text);

entity_t createButton(const char* label, const std::function<void()>& fn);

}


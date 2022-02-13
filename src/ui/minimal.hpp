#pragma once

#include <ecx/ecx.hpp>
#include <functional>

namespace ek {

struct Text2D;

Text2D& addText(ecs::Entity e, const char* text);

ecs::Entity createButton(const char* label, const std::function<void()>& fn);

}


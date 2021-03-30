#pragma once

#include <ecxx/ecxx.hpp>
#include <functional>

namespace ek {

class Text2D;

Text2D& addText(ecs::EntityApi e, const char* text);

ecs::EntityApi createButton(const char* label, const std::function<void()>& fn);

}


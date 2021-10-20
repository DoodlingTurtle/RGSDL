#pragma once

#include "./ui/Textlayer.h"
#include <functional>

namespace RGSDL::UI {

void setLabelTextLayer(RGSDL::UI::Textlayer);

void registerButton(
    int id,
    int x, int y,
    int w, int h,
    const SDL_Color& inactiveColor,
    const SDL_Color& activeColor,
    const char* label, const std::function<void(int id, int x, int y, bool pressed)> onClick);

void onUpdate(RGSDL::Engine* game);

void onDraw(RGSDL::Engine* game);

void _cleanup(RGSDL::Engine* game);

}

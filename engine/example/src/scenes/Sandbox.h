#pragma once

#include "../engine/Engine.h"
#include "../engine/Transform.h"
#include "../engine/Tilemap.h"
#include "../engine/ui/Textlayer.h"
#include "../engine/Scene.h"

using namespace RGSDL;

namespace Scenes {

    class Sandbox : public RGSDL::Scene {
        public:
            Sandbox();

            bool onStart(RGSDL::Engine* game) override;
            bool onUpdate(Engine* game, float deltaTime) override;
            void onDraw(Engine* game) override;
            Scene* onEnd(RGSDL::Engine* game) override;

        private:
            bool keep_running;
            Vec2<int> lastMouse;
            bool mousePressed;

            Texture sheet;
            Texture font;
            Texture ship;
            Texture enemy;

            Tilemap map;
            UI::Textlayer text;

            Transform tr_ship;
            Transform tr_enemy;
            Vec2<int> tr_toplayer;

            unsigned char layer_sprites;
            unsigned char layer_tilemap;
            unsigned char layer_text;
    };
}

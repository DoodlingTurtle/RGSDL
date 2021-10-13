#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>


namespace RGSDL {

    class Engine;
    class TilemapData;
    class Transform;

    struct Texture {
        SDL_Texture* sdl_texture;
        SDL_Rect crop = {0, 0, 0, 0};
        const Texture* parent;
        Engine* game;

        Texture clone() const ;
        void draw(const Transform& tr); 

        private:
            SDL_Point originalSize;

            friend class Engine;
            friend class TilemapData;
    };

}

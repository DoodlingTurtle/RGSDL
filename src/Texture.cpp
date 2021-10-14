#include "../Texture.h"
#include "../Engine.h"

namespace RGSDL {
    Texture Texture::clone() const {
        Texture t;

        t.crop.x = crop.x;
        t.crop.y = crop.y;
        t.crop.w = crop.w;
        t.crop.h = crop.h;

        t.originalSize.x = originalSize.x;
        t.originalSize.y = originalSize.y;

        t.game = game;

        if(this->parent) t.parent = this->parent;
        else             t.parent = this;

        t.sdl_texture = nullptr;

        return t;
    }

    void Texture::draw(const Transform &tr) {

        SDL_Texture* stx;
        if(parent && parent->sdl_texture) stx = parent->sdl_texture;
        else if(!parent && sdl_texture)   stx = sdl_texture;
        else {
            Error("failed to draw deleted texture");
            return;
        }

        SDL_Rect dst;
        dst.x = tr.position.x - tr.origin.x;
        dst.y = tr.position.y - tr.origin.y;
        dst.w = crop.w;
        dst.h = crop.h;

        SDL_RenderCopyEx(game->renderer, stx, &crop, &dst, tr.rotation.getDegree(), &tr.origin, SDL_RendererFlip::SDL_FLIP_NONE);
    }

}

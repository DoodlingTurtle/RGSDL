#include "../Tilemap.h"
#include "../Engine.h"
#include "../Utils.h"
#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <alloca.h>

namespace RGSDL {
    //=============================================================================
    // TileMap
    //=========================================================================={{{

    TilemapData::TilemapData() 
        : mapSize({0, 0}), tileCnt(0), tileIndices(nullptr)
        , dirty(true), game(nullptr), position({0, 0}), tileset()
        , tilesetTilesPerRow(0), rendered_map()
        { this->tileIndices = (int*)malloc(sizeof(int)); }

    TilemapData::TilemapData(
            Engine* eg,
            const Texture& ts,
            const int* tileIndices,
            int mtw, int mth,
            int tw, int th
            ) 
        : mapSize({mtw, mth}), tileCnt(mtw * mth), tileIndices(nullptr)
        ,game(eg), position({0, 0}), tileset(ts.clone())
        ,tilesetTilesPerRow(ts.originalSize.x / tw)
        ,rendered_map(), dirty(true)
        {
            int pw = game->viewPortSize.x;
            int ph = game->viewPortSize.y;

            this->tileIndices = (int*)malloc(sizeof(int) * tileCnt);
            memcpy(this->tileIndices, tileIndices, sizeof(int) * tileCnt);

            tileset.crop.x = 0;
            tileset.crop.y = 0;
            tileset.crop.w = tw;
            tileset.crop.h = th;

            rendered_map.sdl_texture = SDL_CreateTexture(
                    game->renderer, 
                    SDL_PIXELFORMAT_RGBA8888, 
                    SDL_TEXTUREACCESS_TARGET, pw, ph);

            if(!rendered_map.sdl_texture) {
                throw "failed to create TilemapData::rendered_map texture";
            }

            // Important, otherwise the Map will not have any alpha channels 
            SDL_SetTextureBlendMode(rendered_map.sdl_texture, SDL_BLENDMODE_BLEND);

            rendered_map.crop.x = 0;
            rendered_map.crop.y = 0;
            rendered_map.crop.w = pw;
            rendered_map.crop.h = ph;

        }

    void TilemapData::draw(Engine* game) {
        rerender(game);
        SDL_RenderCopy(game->renderer, rendered_map.sdl_texture, &rendered_map.crop, &rendered_map.crop);
    }

    void TilemapData::changeMapTile(int x, int y, int newTileIndex) {
        tileIndices[y * mapSize.x + x] = newTileIndex;
        //TODO: Only set dirty, if changed tile is within few
        dirty = true;
    } 

    void TilemapData::changeTileSet(const Texture &ts, int tw, int th) { /*{{{*/

        tileset = ts.clone();
        tilesetTilesPerRow = ts.originalSize.x/tw;
        tileset.crop.x = 0;
        tileset.crop.y = 0;
        tileset.crop.w = tw;
        tileset.crop.h = th;

        dirty = true;
    } /*}}}*/

    void TilemapData::scrollMap(int distX, int distY)  {
        position.x += distX;
        position.y += distY;
        dirty = true;
    }
    void TilemapData::scrollMap(const Vec2<int>& distance) {
        position.x += distance.x;
        position.y += distance.y;
        dirty = true;
    }

    void TilemapData::setPosition(int x, int y) {
        position.x = x;
        position.y = y;
        dirty = true;
    }
    void TilemapData::setPosition(const Vec2<int>& pos) {
        position.x = pos.x;
        position.y = pos.y;
        dirty = true;
    }

    void TilemapData::rerender(Engine* game) {
        int index = 0, tx = 0, ty = 0;
        if(!dirty or !game) return;

        SDL_Texture* target = SDL_GetRenderTarget(game->renderer);

        SDL_SetRenderTarget(game->renderer, rendered_map.sdl_texture);
        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 0);
        SDL_RenderClear(game->renderer);

        SDL_Rect dst;
        dst.w = tileset.crop.w;
        dst.h = tileset.crop.h;

        // find start/end x tile
        index = (int)((float)game->viewPortSize.x /(float)tileset.crop.w);
        int stx = 0-(int)(position.x / (float)tileset.crop.w);
        int etx = index + stx +1;

        // find start/end y tile
        index = (int)((float)game->viewPortSize.y /(float)tileset.crop.h);
        int sty = 0-(int)(position.y / (float)tileset.crop.h);
        int ety = index + sty + 1;

        // clamp start and end to the map size
        if(stx < 0) { stx = 0; } if(stx > mapSize.x) { stx = mapSize.x; }
        if(etx < 0) { etx = 0; } if(etx > mapSize.x) { etx = mapSize.x; }
        if(sty < 0) { sty = 0; } if(sty > mapSize.y) { sty = mapSize.y; }
        if(ety < 0) { ety = 0; } if(ety > mapSize.y) { ety = mapSize.y; }

        //Debug(sx << "x" << sy << "     " << stx << "x" << etx << " to " << sty << "x" << ety);

        for(int y = sty; y < ety; y++) {
            for(int x = stx; x < etx; x++) {

                index = tileIndices[y * mapSize.x + x];
                ty =(int)((float)index / (float)tilesetTilesPerRow);  
                tx = index - ty * tilesetTilesPerRow;

                tileset.crop.x = tileset.crop.w * tx; 
                tileset.crop.y = tileset.crop.h * ty; 

                dst.x = x * tileset.crop.w + position.x;
                dst.y = y * tileset.crop.h + position.y;

                SDL_RenderCopy(game->renderer, tileset.parent->sdl_texture, &tileset.crop, &dst);
            } 
        }

        SDL_SetRenderTarget(game->renderer, target);
        dirty = false;

    }

    TilemapData::~TilemapData() {
        if(rendered_map.sdl_texture)
            SDL_DestroyTexture(rendered_map.sdl_texture);
        free(tileIndices);
    }

}
//}}}


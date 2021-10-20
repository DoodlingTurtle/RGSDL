#pragma once
#include "./Texture.h"
#include "./Transform.h"
#include "./Vec2.h"
#include <vector>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

namespace RGSDL {

    class Engine;

    class TilemapData{
        public:
            TilemapData();

            void changeTileSet(const Texture& newTileset, int tilePixelWidth, int tilePixelHeight);
            void changeMapTile(int x, int y, int newTileIndex);

            void scrollMap(int distX, int distY);
            void scrollMap(const Vec2<int>& distance);
            
            void setPosition(int x, int y);
            void setPosition(const Vec2<int>&);

            void draw(Engine* game);

            virtual ~TilemapData();

        protected:
            TilemapData(Engine* game, const Texture&, const int*, int, int, int, int);

            SDL_Point mapSize;
            int tileCnt;
            int* tileIndices;
            bool dirty;

            Texture tileset; 

        private:
            Engine* game;
            Vec2<int> position;

            int tilesetTilesPerRow;

            /** needs to be called when ever the tileset changes */
            void rerender(Engine* game);

            Texture rendered_map; 


            friend class Engine;
    };

    typedef TilemapData* Tilemap;
}

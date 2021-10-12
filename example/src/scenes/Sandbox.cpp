#include "./Sandbox.h"
#include <SDL2/SDL_keycode.h>
#include <algorithm>
#include <cstdio>
#include <iterator>

namespace Scenes {

    Sandbox::Sandbox() 
        : RGSDL::Scene(true) 
          , keep_running(true), lastMouse({0}), mousePressed(false)
          , map(nullptr), text(nullptr)
          { }

    bool Sandbox::onStart(RGSDL::Engine *game) { /*{{{ */

        game->backgroundColor = {81, 139, 251, 255};
        game->borderColor = {85, 38, 108, 255};

        sheet = game->loadTexture("./assets/ship.png"); 
        ship = sheet.clone();
        ship.crop.w = 32;
        ship.crop.h = 32;

        enemy = ship.clone();
        enemy.crop.y += 64;


        tr_ship = { {32, 32}, {16, 16}, {1.0f, 1.0f}, 0.0f };
        tr_enemy = tr_ship;
        tr_enemy.position = {256 - 32, 192 - 32};

        font = game->loadTexture("./assets/gnsh-bitmapfont-colour8.png");

        int mapData[6*8];
        std::fill(std::begin(mapData), std::begin(mapData)+(6*8), 1);
        map = game->createTilemap(sheet, mapData, 32, 32, 8, 6);

        text = game->createTextlayer(font, "Hello\n  World", 5, 12, 51, 16);
        text->setPositon(8, 8);

        return true;
    } /*}}}*/

    bool Sandbox::onUpdate(RGSDL::Engine* game, float dt) { /*{{{*/

        if(game->keyReleased(SDLK_ESCAPE))
            return false;

        if(game->mousePressed(SDL_BUTTON_LEFT)) {
            int tx = (int)(game->mousePosition.x / 32.0f);
            int ty = (int)(game->mousePosition.y / 32.0f);

            char buffer[7];
            sprintf(buffer, "%dx%d", tx, ty);
            text->setText(buffer);

            map->changeMapTile(tx, ty, 0);
        }

        return true;
    } /*}}}*/

    void Sandbox::onDraw(RGSDL::Engine* game) { /*{{{*/
        map->draw(game);
        game->switchLayer(1);
        ship.draw(tr_ship);

        game->switchLayer(255);
        game->clear();
        text->draw(game);
    } /*}}}*/

    Scene* Sandbox::onEnd(RGSDL::Engine *game) { /*{{{*/
        game->destroyTilemap(map);
        game->destroyTexture(sheet);
        game->destroyTexture(font);

        game->clearAll();

        return nullptr;
    } /*}}}*/



}

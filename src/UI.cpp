#include "../UI.h"
#include "./Textlayer.cpp"
#include "../Engine.h"
#include "../Utils.h"
#include <unordered_set>
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#define CNT_MAX_BUTTONS 128

namespace RGSDL::UI {

    struct ButtonRect {
        int id;
        SDL_Rect rect;
        std::function<bool(int id, int x, int y, bool pressed)> onClick;
    };

    struct ButtonRender {
        SDL_Rect rect;
        unsigned char cr, cg, cb, ca;
        unsigned char ar, ag, ab, aa;
        std::string label = "";
    };

    static int cntButtons = 0;
    static ButtonRect   buttonrects[CNT_MAX_BUTTONS];
    static ButtonRender buttonlist[CNT_MAX_BUTTONS];
    static std::unordered_set<int> activeButtons;
    static RGSDL::UI::Textlayer button_label = nullptr;

    static unsigned char backlayer = 253; // Button background, etc.
    static unsigned char activelayer = 254; // cleared every frame, draws "active" - graphics for buttons
    static unsigned char textlayer = 255; // shows text

    //TODO: Add function to change the 3 default layers

    static bool dirty = false;

    void setLabelTextLayer(RGSDL::UI::Textlayer button_labels) {
        button_label = button_labels;
    }

    void registerButton(
            int id,
            int x, int y,
            int w, int h,
            const SDL_Color& inactiveColor,
            const SDL_Color& activeColor,
            const char* label, 
            const std::function<bool(int id, int x, int y, bool pressed)> onClick)
    {
        if (CNT_MAX_BUTTONS > cntButtons) {

            buttonlist[cntButtons].rect.x = x;
            buttonlist[cntButtons].rect.y = y;
            buttonlist[cntButtons].rect.w = w;
            buttonlist[cntButtons].rect.h = h;

            buttonlist[cntButtons].cr = inactiveColor.r;
            buttonlist[cntButtons].cg = inactiveColor.g;
            buttonlist[cntButtons].cb = inactiveColor.b;
            buttonlist[cntButtons].ca = inactiveColor.a;

            buttonlist[cntButtons].ar = activeColor.r;
            buttonlist[cntButtons].ag = activeColor.g;
            buttonlist[cntButtons].ab = activeColor.b;
            buttonlist[cntButtons].aa = activeColor.a;

            buttonlist[cntButtons].label += std::string(label);

            buttonrects[cntButtons].id = id;
            buttonrects[cntButtons].rect.x = x;
            buttonrects[cntButtons].rect.y = y;
            buttonrects[cntButtons].rect.w = w;
            buttonrects[cntButtons].rect.h = h;
            buttonrects[cntButtons].onClick = onClick;

            cntButtons++;

            dirty = true;

        } else
            Error("Cant add anymore buttons to the ui limit of " << CNT_MAX_BUTTONS << " has is reached");
    }

    void onUpdate(RGSDL::Engine* game)
    {
        Vec2<int> mp = game->mousePosition;
        bool mouseBtn = (game->mousePressed(SDL_BUTTON_LEFT) || game->mouseHeld(SDL_BUTTON_LEFT));

        std::unordered_set<int>::const_iterator oldState;
        if(mouseBtn) {
            bool newState;
            for(int a = 0; a < cntButtons; a++) {
                oldState = activeButtons.find(a);
                newState = (
                        mp.x >= buttonrects[a].rect.x && 
                        mp.x <= buttonrects[a].rect.x+buttonrects[a].rect.w &&
                        mp.y >= buttonrects[a].rect.y &&
                        mp.y <= buttonrects[a].rect.y+buttonrects[a].rect.h
                        ); 

                if(newState && oldState == activeButtons.end()) {
                    if(buttonrects[a].onClick(buttonrects[a].id, mp.x, mp.y, true))
                        activeButtons.emplace(a);
                }
            } 
        }
        else {
            for(int a : activeButtons)
                buttonrects[a].onClick(buttonrects[a].id, mp.x, mp.y, false);

            activeButtons.clear();
        }
    }

    void onDraw(RGSDL::Engine* game)
    {
        Uint8 r, g, b, a;
        SDL_Renderer* renderer = game->renderer;
        SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
        unsigned char layer_backup = game->getCurrentLayer();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_SetRenderTarget(renderer, game->layers[activelayer]);
        SDL_RenderClear(renderer);

        for(int a : activeButtons) {
            SDL_SetRenderDrawColor(renderer, buttonlist[a].ar, buttonlist[a].ag, buttonlist[a].ab, buttonlist[a].aa);
            SDL_RenderFillRect(renderer, &buttonlist[a].rect);    
        }

        if (dirty) {

            SDL_SetRenderTarget(renderer, game->layers[backlayer]);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            SDL_RenderClear(renderer);

            for(int a = 0; a < cntButtons; a++) {
                SDL_SetRenderDrawColor(renderer, buttonlist[a].cr,buttonlist[a].cg,buttonlist[a].cb,buttonlist[a].ca);
                SDL_RenderFillRect(renderer, &buttonlist[a].rect);    
            }

            if(button_label != nullptr) {
                SDL_SetRenderTarget(renderer, game->layers[textlayer]);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                SDL_RenderClear(renderer);

                for(int a = 0; a < cntButtons; a++) {
                    button_label->setText(buttonlist[a].label.c_str());
                    button_label->setPosition(
                        buttonlist[a].rect.x + (buttonlist[a].rect.w - button_label->getTextPXWidth()) / 2,
                        buttonlist[a].rect.y + (buttonlist[a].rect.h - button_label->getTextPXHeight()) / 2);
                    button_label->draw(game);
                }
                dirty = false;
            }

        }

        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        SDL_SetRenderTarget(renderer, game->layers[backlayer]);
    }

}

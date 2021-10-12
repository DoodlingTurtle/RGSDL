#include "../Engine.h"
#include "../Macros.h"
#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_pixels.h>
#include <algorithm>
#include <iterator>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <cstring>
#include <vector>

namespace RGSDL {

    //=============================================================================
    // Member functions
    //=========================================================================={{{
    Engine::Engine() 
        : backgroundColor({0, 0, 0, 255})
        , borderColor({0, 0, 0, 255})
        , mousePosition({0, 0})
        , currentScene(nullptr), window(nullptr), renderer(nullptr)
        , _ticks_passed(0), _deltaTime(0.0f)
        , layers(), currentLayer(0) { /*{{{*/
            memset(layers, 0, sizeof(SDL_Texture*)*256); 
        } /*}}}*/


    int Engine::start(
            int& argc, char**& argv,
            int windowWidth, int windowHeight, 
            float targetDPI,
            const char *title, 
            Scene* initialScene,
            uint32_t sdl_flags
            ) { /*{{{*/

        if(initialScene == nullptr) { /*{{{*/
            Error("initial scene cant be null");
            return -1;
        } /*}}}*/
        if(SDL_Init(SDL_INIT_VIDEO)) { /*{{{*/
            Error("SDL-Init error: " << SDL_GetError()); 
            return -1;
        } /*}}}*/;
        if(IMG_Init(IMG_INIT_PNG) == 0) { /*{{{*/
            Error("Failed to initialize SDL2_Img: " << IMG_GetError());
            return -1;
        } /*}}}*/

        float ddpi;
        SDL_GetDisplayDPI(0, &ddpi, nullptr, nullptr);
        float ws = ceil(ddpi / targetDPI);

        currentScene = initialScene;
        window = SDL_CreateWindow(title
                , SDL_WINDOWPOS_CENTERED_DISPLAY(0)
                , SDL_WINDOWPOS_CENTERED_DISPLAY(0)
                , windowWidth * ws, windowHeight * ws
                , SDL_WINDOW_RESIZABLE|SDL_WINDOW_SHOWN);

        if(window == nullptr) { /*{{{*/
            Error("SDL-WindowError: " << SDL_GetError());
            return -1;
        } /*}}}*/

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
        if(renderer == nullptr) { /*{{{*/
            Error("SDL-CreateRendererError: " << SDL_GetError());
            return -1;
        } /*}}}*/

        viewPortSize = { windowWidth, windowHeight };
        SDL_RenderSetLogicalSize(renderer, windowWidth, windowHeight);

        screenRect.x = 0; screenRect.y = 0;
        screenRect.w = viewPortSize.x; screenRect.h = viewPortSize.y;

        for(int a = 0; a < 256; a++) { /*{{{*/
            layers[a] = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, windowWidth, windowHeight);
            SDL_SetRenderTarget(renderer, layers[a]);
            SDL_SetTextureBlendMode(layers[a], SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            SDL_RenderClear(renderer);
        } /*}}}*/


        for(int a = 1; a < argc; a++){
            if(!strcmp(argv[a], "--fullscreen"))
                this->toggleFullscreen();
        }

        SDL_SetRenderTarget(renderer, layers[0]);

        while(tick()) {}

        return 0;
    } /*}}}*/

    bool Engine::tick() { /*{{{*/

        if(!currentScene) return false;
        else if(!currentScene->started) {
            if(!currentScene->onStart(this))
                return false;

            currentScene->started = true;
        }

        uint32_t ticks = SDL_GetTicks();
        _deltaTime = ticks - _ticks_passed;
        _ticks_passed = ticks;

        // if this is the first Engine-tick or if the delta-time 
        // counter rolled over, then stop time for one frame
        // and reset the counter 
        if(_deltaTime == ticks || _deltaTime < 0) {
            _deltaTime = 0.0000001f;
            return true;
        }
        else _deltaTime /= 1000.0f;


        keys_pressed.clear();
        keys_released.clear();
        mouse_pressed.clear();
        mouse_released.clear();

        SDL_Event event;
        while(SDL_PollEvent(&event)) {/*{{{*/
            switch(event.type) {
                case SDL_MOUSEBUTTONDOWN: {
                                              mouse_pressed.emplace(event.button.button);
                                          } break;

                case SDL_MOUSEBUTTONUP: {
                                            mouse_released.emplace(event.button.button);
                                            auto it = mouse_held.find(event.key.keysym.sym);
                                            if (it != mouse_held.end())
                                                mouse_held.erase(it);
                                        } break;

                case SDL_KEYDOWN: {
                                      if (event.key.keysym.sym != last_pressed)
                                          keys_pressed.emplace(event.key.keysym.sym);

                                      last_pressed = event.key.keysym.sym;
                                  } break;

                case SDL_KEYUP: {
                                    keys_released.emplace(event.key.keysym.sym);
                                    auto it = keys_held.find(event.key.keysym.sym);
                                    if (it != keys_held.end())
                                        keys_held.erase(it);

                                    if (event.key.keysym.sym == last_pressed)
                                        last_pressed = 0;
                                } break;

                case SDL_MOUSEMOTION:
                                mousePosition = {
                                    event.motion.x, event.motion.y
                                };
                                break;

                case SDL_QUIT: return false; break;
            }
        }/*}}}*/

        if(!currentScene->onUpdate(this, _deltaTime)) { /*{{{*/
            currentScene->started = false;
            Scene* next = currentScene->onEnd(this);
            if(!currentScene->persistent) delete currentScene;

            currentScene = next;
            return true;
        } /*}}}*/ 


        currentScene->onDraw(this);

        SDL_SetRenderTarget(renderer, nullptr);
        SDL_SetRenderDrawColor(renderer ,borderColor.r, borderColor.g, borderColor.b, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer ,backgroundColor.r , backgroundColor.g, backgroundColor.b, backgroundColor.a);
        SDL_Rect r = {0, 0, viewPortSize.x, viewPortSize.y};
        SDL_RenderFillRect(renderer, &r);

        for(int a=0; a < 256; a++)
            SDL_RenderCopy(renderer, layers[a], &screenRect, &screenRect);

        SDL_RenderPresent(renderer);
        SDL_SetRenderTarget(renderer, layers[0]);

        return true;
    }/*}}}*/

    Engine::~Engine() { /*{{{*/

        for(int a = 0; a < 256; a++)
            SDL_DestroyTexture(layers[a]);

        if(currentScene && !currentScene->persistent) 
            delete currentScene;

        if(renderer) SDL_DestroyRenderer(renderer);

        if(window)   SDL_DestroyWindow(window);

        IMG_Quit();
        SDL_Quit();

    } /*}}}*/

    bool Engine::keyPressed(SDL_Keycode code) { return keys_pressed.find(code) != keys_pressed.end(); }
    bool Engine::keyHeld(SDL_Keycode code)    { return keys_held.find(code) != keys_held.end(); }
    bool Engine::keyReleased(SDL_Keycode code) { return keys_released.find(code) != keys_released.end(); }

    bool Engine::mousePressed(uint8_t code) { return mouse_pressed.find(code) != mouse_pressed.end(); }
    bool Engine::mouseHeld(uint8_t code)    { return mouse_held.find(code) != mouse_held.end(); }
    bool Engine::mouseReleased(uint8_t code) { return mouse_released.find(code) != mouse_released.end(); }

    void Engine::toggleFullscreen() {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }

    //}}}

    //=============================================================================
    // Layer functions
    //=========================================================================={{{

    void Engine::switchLayer(unsigned char zLayer) {
        currentLayer = zLayer;
        SDL_SetRenderTarget(renderer, layers[zLayer]);
    }

    void Engine::clear() {
        Uint8 r, g, b, a;
        SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
    }

    void Engine::clearAll() {
        Uint8 r, g, b, a;
        SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

        for(int a = 0; a < 256; a++) {
            SDL_SetRenderTarget(renderer, layers[a]);
            SDL_RenderClear(renderer);
        }

        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        SDL_SetRenderTarget(renderer, layers[currentLayer]);
    }

    //}}}

    //=============================================================================
    // Texture-Functions
    //=========================================================================={{{
    Texture Engine::loadTexture(const char *filename) {/*{{{*/

        Texture t;
        t.crop.x = 0;
        t.crop.y = 0;

        t.sdl_texture = IMG_LoadTexture(renderer, filename);

        if(t.sdl_texture == nullptr) {
            Error("failed to load image " << IMG_GetError());
            throw "img_load_error";
        }
        SDL_QueryTexture(t.sdl_texture, nullptr, nullptr, &t.crop.w, &t.crop.h);

        t.originalSize.x = t.crop.w;
        t.originalSize.y = t.crop.h;
        t.parent = nullptr;
        t.game = this;

        Debug("orig-size: " << t.originalSize.x << "x" << t.originalSize.y);

        return t;

    }/*}}}*/

    void Engine::destroyTexture(Texture &tex) {  /*{{{*/

        if(!tex.parent)
            SDL_DestroyTexture(tex.sdl_texture); 

        tex.sdl_texture = nullptr;
        tex.crop.x = 0;
        tex.crop.y = 0;
        tex.crop.w = 0;
        tex.crop.h = 0;

        tex.originalSize.x = 0;
        tex.originalSize.y = 0;
        tex.parent = nullptr;

    } /*}}}*/
    //}}}

    //=============================================================================
    // Tilemap functions
    //=========================================================================={{{
    Tilemap Engine::createTilemap(
            const Texture& tileset, const int* tileIndices
            , int tilePixelWidth, int tilePixelHeight
            , int mapTilesWidth, int mapTilesHeight
            ) {
        Tilemap map = new TilemapData(
                this, tileset, 
                tileIndices,
                mapTilesWidth, mapTilesHeight,
                tilePixelWidth, tilePixelWidth
                );
        return map;
    }

    void Engine::destroyTilemap(Tilemap map) {
        delete map;
    }
    //}}}

    //=============================================================================
    // Textlayer function
    //=========================================================================={{{
    UI::Textlayer Engine::createTextlayer(
            const Texture& font, const char* tx 
            , int fontPxWidth, int fontPxHeight
            , int maxLettersPerRow, int maxRows
            ) {

        int slots = maxLettersPerRow * maxRows;
        int buffer[slots];
        memset(buffer, 0, sizeof(unsigned int)*slots);

        UI::Textlayer text = new UI::TextlayerData(
                this, font, buffer
                ,fontPxWidth, fontPxHeight
                ,maxLettersPerRow, maxRows
                );

        text->setText(tx);

        return text;
    }
    void Engine::destroyTextlayer(UI::Textlayer text) {
        delete text;
    }
    //}}}

}

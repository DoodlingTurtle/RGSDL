#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <unordered_set>
#include <vector>
#include "./Texture.h"
#include "./Tilemap.h"
#include "./Transform.h"
#include "./Vec2.h"
#include "./Scene.h"

#include "./ui/Textlayer.h"

namespace RGSDL {

    class Engine { /*{{{*/
        public:
            Engine();

            //=============================================================================
            // Engine Setup 
            //=========================================================================={{{
            /** \brief aktivates an Engine - Instance and opens it in a Window
             * \param argc - argc passed to main
             * \param argv - argv passed to main
             * \param viewPortWidth = width in px (not the window width)
             * \param viewPortHight = height in px (not the window height)
             * \param targetDPI = will be combined with the viewPort size and actual Screen DPI
             *                    to define how big the window will be 
             *                    (example: a 256x192 viewport with 103 dpi 
             *                     will should open a ~4 inch window (NDSi XL size))
             * \param initialScene = each Engine requires a scene to run.
             *                       if no scene is provided, then nothing will happen
             *
             * \param sdl_flags = additional SDL_Flags for SDL_Init
             *                    SDL_INIT_VIDEO will always be on by default
             *
             * \param window_flags = defines additional SDL_WINDOW - Flags
             *                    SDL_WINDOW_RESIZABLE|SDL_WINDOW_SHOWN are always set
             *
             * \return the created Engine instance 
             */

            int start(
                    int& argc, char**& argv,
                    int viewPortWidth, int viewPortHight, 
                    float targetDPI,
                    const char *title, 
                    Scene* initialScene,
                    uint32_t sdl_flags = 0,
                    uint32_t window_flags=0
                    );

            SDL_Color backgroundColor;
            SDL_Color borderColor;

            Vec2<int> mousePosition;
            //}}}

            //=============================================================================
            // Window Controlls
            //=========================================================================={{{
            void toggleFullscreen();
            //}}}


            //=============================================================================
            // Drawing functions
            //=========================================================================={{{
          
            /** switches the Target for all following Draw calls
             * \param zLayer = one of the 256 available layers 
             *                 0 = lowest, 255 = highest
             */
            void switchLayer(unsigned char zLayer);
            
            /** \brief clears the contents of the currently selected drawing layer
             *         (selected via Engine::switchLayer)
             */
            void clear();

            /** \brief clears the contents of all 256 Layer of the Engine
             */
            void clearAll();
            //}}}

            //=============================================================================
            // Texture functions 
            //=========================================================================={{{
            Texture loadTexture(const char* filename);
            void destroyTexture(Texture& tex);
            //}}}

            //=============================================================================
            // Tilemap functions
            //=========================================================================={{{
            Tilemap createTilemap(
                    const Texture& tileset, const int* tileIndices
                    , int tilePixelWidth, int tilePixelHeight
                    , int mapTilesWidth, int mapTilesHeight
                    );
            void destroyTilemap(Tilemap map);
            //}}}

            //=============================================================================
            // Textlayer functions
            //=========================================================================={{{
            UI::Textlayer createTextlayer(
                    const Texture& font, const char* text 
                    , int fontPxWidth, int fontPxHeight
                    , int maxLettersPerRow, int maxRows
                    );
            void destroyTextlayer(UI::Textlayer text);
            //}}}

            //=============================================================================
            // Input function
            //=========================================================================={{{
            bool keyPressed(SDL_Keycode code);
            bool keyHeld(SDL_Keycode code);
            bool keyReleased(SDL_Keycode code);

            bool mousePressed(Uint8 code);
            bool mouseHeld(Uint8 code);
            bool mouseReleased(Uint8 code);
            //}}}

            ~Engine();

            //=============================================================================
            // Privates
            //=========================================================================={{{
        private: 

            /** \brief ticks the current active Engine by exactly once cycle */
            bool tick();

            Scene* currentScene;
            SDL_Window*   window;
            SDL_Renderer* renderer;

            Vec2<int> viewPortSize;
            SDL_Rect screenRect;

            uint32_t _ticks_passed;
            float _deltaTime;

            void RenderAll();

            SDL_Keycode last_pressed;
            std::unordered_set<SDL_Keycode> keys_pressed;
            std::unordered_set<SDL_Keycode> keys_held;
            std::unordered_set<SDL_Keycode> keys_released;

            std::unordered_set<uint8_t> mouse_pressed;
            std::unordered_set<uint8_t> mouse_held;
            std::unordered_set<uint8_t> mouse_released;

            unsigned char currentLayer;
            SDL_Texture* layers[256];

            friend class TilemapData;
            friend class Texture;

            //            void _applyScreenChange();
            //}}}
    } /*}}}*/;
}

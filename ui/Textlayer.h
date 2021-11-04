#pragma once

#include "../Texture.h"
#include "../Tilemap.h"

namespace RGSDL {
    class Engine;
};

namespace RGSDL::UI {

    class TextlayerData : public RGSDL::TilemapData {
        public:

            void setText(const char* characters);
            int getTextPXWidth();
            int getTextPXHeight();

        private:
            int pxw, pxh;

            TextlayerData(
                    Engine* game 
                    , const Texture &font
                    , const int* buffer
                    , int fontPxWith, int fontPxHeight
                    , int maxLetterPerRow, int maxRows
                    );

            friend class RGSDL::Engine;


    };

    typedef TextlayerData* Textlayer;

}

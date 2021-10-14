#include "../headers/ui/Textlayer.h"

namespace RGSDL::UI {

    void TextlayerData::setText(const char *characters) { /*{{{*/

        int x=0, y=0;
        const char* c = &characters[0];

        memset(tileIndices, 0, sizeof(int) * tileCnt);

        while(*c != '\0' ) { /*{{{*/
            if(*c == '\n') {
                y++;
                x=0;
                c++;
                continue;
            }
            else if(x >= mapSize.x) {
                y++;
                x=0;
            }

            if(y >= mapSize.y) break;

            tileIndices[y * mapSize.x + x] = *c - 32;
            x++;
            c++;
        } /*}}}*/
        
        dirty = true;

    } /*}}}*/

    TextlayerData::TextlayerData(
            Engine* game
            , const Texture &font
            , const int* buffer
            , int fontPxWith, int fontPxHeight
            , int maxLettersPerRow, int maxRows
            ) 
        : TilemapData (
                game, font, buffer
                , maxLettersPerRow, maxRows
                , fontPxWith, fontPxHeight
                ) {}

}

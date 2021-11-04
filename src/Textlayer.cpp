#include "../ui/Textlayer.h"

namespace RGSDL::UI {

    void TextlayerData::setText( const char* characters )
    {
        int         x = 0, y = 0;
        const char* c = &characters[ 0 ];

        memset( tileIndices, 0, sizeof( int ) * tileCnt );

        pxw = 0;
        pxh = 0;

        while ( *c != '\0' ) {
            if ( *c == '\n' ) {
                y++;

                if ( pxw < x ) pxw = x;

                x = 0;
                c++;
                continue;
            }
            else if ( x >= mapSize.x ) {
                y++;

                if ( pxw < x ) pxw = x;

                x = 0;
            }

            if ( y >= mapSize.y ) break;

            tileIndices[ y * mapSize.x + x ] = *c - 32;
            x++;
            c++;
        }

        if ( pxw < x ) pxw = x;

        pxw *= tileset.crop.w;
        pxh = ( y + 1 ) * tileset.crop.h;

        dirty = true;
    }

    int TextlayerData::getTextPXWidth() { return pxw; }
    int TextlayerData::getTextPXHeight() { return pxh; }

    TextlayerData::TextlayerData(
        Engine* game, const Texture& font, const int* buffer, int fontPxWith, int fontPxHeight,
        int maxLettersPerRow, int maxRows )
        : TilemapData( game, font, buffer, maxLettersPerRow, maxRows, fontPxWith, fontPxHeight ),
          pxw( 0 ), pxh( 0 )
    {
    }

} // namespace RGSDL::UI

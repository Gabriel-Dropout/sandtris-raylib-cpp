//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
// rTexpacker v2.5 Atlas Descriptor Code exporter v2.0                          //
//                                                                              //
// more info and bugs-report:  github.com/raylibtech/rtools                     //
// feedback and support:       ray[at]raylibtech.com                            //
//                                                                              //
// Copyright (c) 2020-2022 raylib technologies (@raylibtech)                    //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////

#include "raylib.h"

#define ATLAS_ATLAS_IMAGE_PATH      "atlas.png"
#define ATLAS_ATLAS_SPRITE_COUNT    16

// Atlas sprite properties
typedef struct rtpAtlasSprite {
    const char *nameId;
    int originX, originY;
    int positionX, positionY;
    int sourceWidth, sourceHeight;
    int padding;
    bool trimmed;
    int trimRecX, trimRecY, trimRecWidth, trimRecHeight;
} rtpAtlasSprite;

// Atlas sprites array
static rtpAtlasSprite rtpDescAtlas[16] = {
    { "sandtetris1", 28, 34, 0, 0, 140, 198, 0, false, 0, 0, 140, 198 },
    { "sandtetris2", 28, 34, 140, 0, 140, 198, 0, false, 0, 0, 140, 198 },
    { "16x16_I", 8, 8, 448, 0, 16, 16, 0, false, 0, 6, 16, 4 },
    { "16x16_J", 8, 8, 464, 0, 16, 16, 0, false, 2, 4, 12, 8 },
    { "16x16_L", 8, 8, 480, 0, 16, 16, 0, false, 2, 4, 12, 8 },
    { "16x16_O", 8, 8, 496, 0, 16, 16, 0, false, 4, 4, 8, 8 },
    { "16x16_S", 8, 8, 0, 198, 16, 16, 0, false, 2, 4, 12, 8 },
    { "16x16_T", 8, 8, 16, 198, 16, 16, 0, false, 2, 4, 12, 8 },
    { "16x16_Z", 8, 8, 32, 198, 16, 16, 0, false, 2, 4, 12, 8 },
    { "24x24_I", 12, 12, 280, 0, 24, 24, 0, false, 0, 9, 24, 6 },
    { "24x24_J", 12, 12, 304, 0, 24, 24, 0, false, 3, 6, 18, 12 },
    { "24x24_L", 12, 12, 328, 0, 24, 24, 0, false, 3, 6, 18, 12 },
    { "24x24_O", 12, 12, 352, 0, 24, 24, 0, false, 6, 6, 12, 12 },
    { "24x24_S", 12, 12, 376, 0, 24, 24, 0, false, 3, 6, 18, 12 },
    { "24x24_T", 12, 12, 400, 0, 24, 24, 0, false, 3, 6, 18, 12 },
    { "24x24_Z", 12, 12, 424, 0, 24, 24, 0, false, 3, 6, 18, 12 },
};

static rtpAtlasSprite mainUI[2] = {
    { "sandtetris1", 28, 34, 0, 0, 140, 198, 0, false, 0, 0, 140, 198 },
    { "sandtetris2", 28, 34, 140, 0, 140, 198, 0, false, 0, 0, 140, 198 }
};
static rtpAtlasSprite mino_16[7] = {
    { "16x16_I", 8, 8, 448, 0, 16, 16, 0, false, 0, 6, 16, 4 },
    { "16x16_J", 8, 8, 464, 0, 16, 16, 0, false, 2, 4, 12, 8 },
    { "16x16_L", 8, 8, 480, 0, 16, 16, 0, false, 2, 4, 12, 8 },
    { "16x16_O", 8, 8, 496, 0, 16, 16, 0, false, 4, 4, 8, 8 },
    { "16x16_S", 8, 8, 0, 198, 16, 16, 0, false, 2, 4, 12, 8 },
    { "16x16_T", 8, 8, 16, 198, 16, 16, 0, false, 2, 4, 12, 8 },
    { "16x16_Z", 8, 8, 32, 198, 16, 16, 0, false, 2, 4, 12, 8 },
};
static rtpAtlasSprite mino_24[7] = {
    { "24x24_I", 12, 12, 280, 0, 24, 24, 0, false, 0, 9, 24, 6 },
    { "24x24_J", 12, 12, 304, 0, 24, 24, 0, false, 3, 6, 18, 12 },
    { "24x24_L", 12, 12, 328, 0, 24, 24, 0, false, 3, 6, 18, 12 },
    { "24x24_O", 12, 12, 352, 0, 24, 24, 0, false, 6, 6, 12, 12 },
    { "24x24_S", 12, 12, 376, 0, 24, 24, 0, false, 3, 6, 18, 12 },
    { "24x24_T", 12, 12, 400, 0, 24, 24, 0, false, 3, 6, 18, 12 },
    { "24x24_Z", 12, 12, 424, 0, 24, 24, 0, false, 3, 6, 18, 12 },
};

inline void DrawSpriteAtlas(Texture2D &atlas, rtpAtlasSprite sprite, int x, int y, Color color) {
    DrawTexturePro(atlas,
        Rectangle{(float)sprite.positionX, (float)sprite.positionY, (float)sprite.sourceWidth, (float)sprite.sourceHeight},
        Rectangle{(float)x, (float)y, (float)sprite.sourceWidth, (float)sprite.sourceHeight},
        Vector2{(float)sprite.originX, (float)sprite.originY},
        0.f,
        color);
}
inline void DrawSpriteAtlas(Texture2D &atlas, rtpAtlasSprite sprite, int x, int y, float angle, Color color) {
    DrawTexturePro(atlas,
        Rectangle{(float)sprite.positionX, (float)sprite.positionY, (float)sprite.sourceWidth, (float)sprite.sourceHeight},
        Rectangle{(float)x, (float)y, (float)sprite.sourceWidth, (float)sprite.sourceHeight},
        Vector2{(float)sprite.originX, (float)sprite.originY},
        angle,
        color);
}
inline void DrawSpriteAtlas(Texture2D &atlas, rtpAtlasSprite sprite, int x, int y, float xscale, float yscale, float angle, Color color) {
    DrawTexturePro(atlas,
        Rectangle{(float)sprite.positionX, (float)sprite.positionY, (float)sprite.sourceWidth, (float)sprite.sourceHeight},
        Rectangle{(float)x, (float)y, (float)sprite.sourceWidth*xscale, (float)sprite.sourceHeight*yscale},
        Vector2{(float)sprite.originX*xscale, (float)sprite.originY*yscale},
        angle,
        color);
}
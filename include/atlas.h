#ifndef ATLAS_H
#define ATLAS_H

#include "raylib.h"
#include "raylib-cpp.hpp"

#define ATLAS_ATLAS_IMAGE_PATH      "atlas.png"
#define ATLAS_ATLAS_SPRITE_COUNT    39

extern raylib::Texture atlas;

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
static rtpAtlasSprite rtpDescAtlas[ATLAS_ATLAS_SPRITE_COUNT] = {
    { "logo", 64, 64, 280, 0, 128, 128, 0, false, 4, 10, 119, 108 },
    { "sandtetris1", 28, 34, 140, 0, 140, 198, 0, false, 0, 0, 140, 198 },
    { "sandtetris2", 28, 34, 0, 0, 140, 198, 0, false, 0, 0, 140, 198 },

    { "16x16_I", 8, 8, 484, 56, 16, 16, 0, false, 0, 6, 16, 4 },
    { "16x16_J", 8, 8, 424, 48, 16, 16, 0, false, 2, 4, 12, 8 },
    { "16x16_L", 8, 8, 468, 56, 16, 16, 0, false, 2, 4, 12, 8 },
    { "16x16_O", 8, 8, 440, 48, 16, 16, 0, false, 4, 4, 8, 8 },
    { "16x16_S", 8, 8, 408, 48, 16, 16, 0, false, 2, 4, 12, 8 },
    { "16x16_T", 8, 8, 480, 40, 16, 16, 0, false, 2, 4, 12, 8 },
    { "16x16_Z", 8, 8, 480, 24, 16, 16, 0, false, 2, 4, 12, 8 },

    { "24x24_I", 12, 12, 408, 24, 24, 24, 0, false, 0, 9, 24, 6 },
    { "24x24_J", 12, 12, 456, 24, 24, 24, 0, false, 3, 6, 18, 12 },
    { "24x24_L", 12, 12, 408, 0, 24, 24, 0, false, 3, 6, 18, 12 },
    { "24x24_O", 12, 12, 432, 24, 24, 24, 0, false, 6, 6, 12, 12 },
    { "24x24_S", 12, 12, 480, 0, 24, 24, 0, false, 3, 6, 18, 12 },
    { "24x24_T", 12, 12, 456, 0, 24, 24, 0, false, 3, 6, 18, 12 },
    { "24x24_Z", 12, 12, 432, 0, 24, 24, 0, false, 3, 6, 18, 12 },

    { "brick0", 0, 0, 441, 88, 9, 9, 0, false, 0, 0, 9, 9 },
    { "brick1", 0, 0, 496, 84, 9, 9, 0, false, 0, 0, 9, 9 },
    { "brick2", 0, 0, 432, 88, 9, 9, 0, false, 0, 0, 9, 9 },
    { "brick3", 0, 0, 487, 84, 9, 9, 0, false, 0, 0, 9, 9 },
    { "brick4", 0, 0, 408, 91, 9, 9, 0, false, 0, 0, 9, 9 },
    { "brick5", 0, 0, 450, 88, 9, 9, 0, false, 0, 0, 9, 9 },

    { "combo_0", 0, 0, 441, 64, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_1", 0, 0, 450, 76, 6, 12, 0, false, 0, 0, 6, 12 },
    { "combo_2", 0, 0, 468, 72, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_3", 0, 0, 477, 72, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_4", 0, 0, 486, 72, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_5", 0, 0, 495, 72, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_6", 0, 0, 432, 76, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_7", 0, 0, 441, 76, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_8", 0, 0, 450, 64, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_9", 0, 0, 432, 64, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_x", 0, 0, 468, 84, 10, 11, 0, false, 0, 0, 10, 11 },
    { "combo_C", 0, 0, 408, 64, 11, 16, 0, false, 0, 0, 11, 16 },
    { "combo_o", 0, 0, 478, 84, 9, 11, 0, false, 0, 0, 9, 11 },
    { "combo_m", 0, 0, 408, 80, 12, 11, 0, false, 0, 0, 12, 11 },
    { "combo_b", 0, 0, 419, 64, 9, 16, 0, false, 0, 0, 9, 16 },
    { "combo_screamer", 0, 0, 428, 64, 4, 16, 0, false, 0, 0, 4, 16 },
};

static rtpAtlasSprite logo = { "logo", 64, 64, 280, 0, 128, 128, 0, false, 4, 10, 119, 108 };

static rtpAtlasSprite mainUI[2] = {
    { "sandtetris1", 28, 34, 140, 0, 140, 198, 0, false, 0, 0, 140, 198 },
    { "sandtetris2", 28, 34, 0, 0, 140, 198, 0, false, 0, 0, 140, 198 },
};
static rtpAtlasSprite mino_16[7] = {
    { "16x16_I", 8, 8, 484, 56, 16, 16, 0, false, 0, 6, 16, 4 },
    { "16x16_J", 8, 8, 424, 48, 16, 16, 0, false, 2, 4, 12, 8 },
    { "16x16_L", 8, 8, 468, 56, 16, 16, 0, false, 2, 4, 12, 8 },
    { "16x16_O", 8, 8, 440, 48, 16, 16, 0, false, 4, 4, 8, 8 },
    { "16x16_S", 8, 8, 408, 48, 16, 16, 0, false, 2, 4, 12, 8 },
    { "16x16_T", 8, 8, 480, 40, 16, 16, 0, false, 2, 4, 12, 8 },
    { "16x16_Z", 8, 8, 480, 24, 16, 16, 0, false, 2, 4, 12, 8 },
};
static rtpAtlasSprite mino_24[7] = {
    { "24x24_I", 12, 12, 408, 24, 24, 24, 0, false, 0, 9, 24, 6 },
    { "24x24_J", 12, 12, 456, 24, 24, 24, 0, false, 3, 6, 18, 12 },
    { "24x24_L", 12, 12, 408, 0, 24, 24, 0, false, 3, 6, 18, 12 },
    { "24x24_O", 12, 12, 432, 24, 24, 24, 0, false, 6, 6, 12, 12 },
    { "24x24_S", 12, 12, 480, 0, 24, 24, 0, false, 3, 6, 18, 12 },
    { "24x24_T", 12, 12, 456, 0, 24, 24, 0, false, 3, 6, 18, 12 },
    { "24x24_Z", 12, 12, 432, 0, 24, 24, 0, false, 3, 6, 18, 12 },
};
static rtpAtlasSprite brick[6] = {
    { "brick0", 0, 0, 441, 88, 9, 9, 0, false, 0, 0, 9, 9 },
    { "brick1", 0, 0, 496, 84, 9, 9, 0, false, 0, 0, 9, 9 },
    { "brick2", 0, 0, 432, 88, 9, 9, 0, false, 0, 0, 9, 9 },
    { "brick3", 0, 0, 487, 84, 9, 9, 0, false, 0, 0, 9, 9 },
    { "brick4", 0, 0, 408, 91, 9, 9, 0, false, 0, 0, 9, 9 },
    { "brick5", 0, 0, 450, 88, 9, 9, 0, false, 0, 0, 9, 9 },
};

static rtpAtlasSprite comboImg[] = {
    { "combo_0", 0, 0, 441, 64, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_1", 0, 0, 450, 76, 6, 12, 0, false, 0, 0, 6, 12 },
    { "combo_2", 0, 0, 468, 72, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_3", 0, 0, 477, 72, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_4", 0, 0, 486, 72, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_5", 0, 0, 495, 72, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_6", 0, 0, 432, 76, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_7", 0, 0, 441, 76, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_8", 0, 0, 450, 64, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_9", 0, 0, 432, 64, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_x", 0, 0, 468, 84, 10, 11, 0, false, 0, 0, 10, 11 },
    { "combo_C", 0, 0, 408, 64, 11, 16, 0, false, 0, 0, 11, 16 },
    { "combo_o", 0, 0, 478, 84, 9, 11, 0, false, 0, 0, 9, 11 },
    { "combo_m", 0, 0, 408, 80, 12, 11, 0, false, 0, 0, 12, 11 },
    { "combo_b", 0, 0, 419, 64, 9, 16, 0, false, 0, 0, 9, 16 },
    { "combo_screamer", 0, 0, 428, 64, 4, 16, 0, false, 0, 0, 4, 16 },
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

// it ignores origin of sprite
inline void DrawSpriteAtlas(Texture2D &atlas, rtpAtlasSprite sprite, ::Rectangle slice, int x, int y, float xscale, float yscale, float angle, Color color) {
    DrawTexturePro(atlas,
        Rectangle{(float)sprite.positionX + slice.x, (float)sprite.positionY + slice.y, slice.width, slice.height},
        Rectangle{(float)x, (float)y, slice.width*xscale, slice.height*yscale},
        Vector2{0, 0},
        angle,
        color);
}

#endif
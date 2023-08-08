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
#define ATLAS_ATLAS_SPRITE_COUNT    0

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
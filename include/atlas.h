#ifndef ATLAS_H
#define ATLAS_H

#include<iostream>
#include <map>
#include <string>

#include "raylib.h"
#include "raylib-cpp.hpp"

#define ATLAS_ATLAS_IMAGE_PATH      "atlas.png"
#define ATLAS_ATLAS_SPRITE_COUNT    47


typedef struct AtlasSprite {
    char nameId[128];           // Sprite original filename (without extension)
    int originX, originY;       // Sprite origin (pivot point), useful in some cases
    int positionX, positionY;   // Sprite position in the atlas
    int sourceWidth;            // Sprite source width (before trim)
    int sourceHeight;           // Sprite source height (before trim)
    int padding;                // Sprite padding, must be added to source size
    int trimmed;                // Sprite is trimmed (removed blank space for better packing)
    int trimX, trimY, trimWidth, trimHeight; // Sprite trim rectangle 
    
    // Glyph info, in case sprite is a font character
    // NOTE: This data could probably be ommited 
    int value;                  // Character value (Unicode)
    int offsetX, offsetY;       // Character offset when drawing
    int advanceX;               // Character advance position X
} AtlasSprite;

extern raylib::Texture atlas;
extern std::map<std::string, AtlasSprite> spriteMap;

void initAtlas(const std::string& filename);
inline AtlasSprite& getAtlasSprite(const std::string& nameId)                   {return spriteMap[nameId];}
inline AtlasSprite& getAtlasSprite(const std::string& nameId, int suffix)       {return spriteMap[nameId + std::to_string(suffix)];}
inline AtlasSprite& getAtlasSprite(const std::string_view& nameId)              {return spriteMap[std::string(nameId)];}
inline AtlasSprite& getAtlasSprite(const std::string_view& nameId, int suffix)  {return spriteMap[std::string(nameId) + std::to_string(suffix)];}
inline AtlasSprite& getAtlasSprite(const char* nameId)                          {return spriteMap[std::string(nameId)];}
inline AtlasSprite& getAtlasSprite(const char* nameId, int suffix)              {return spriteMap[std::string(nameId) + std::to_string(suffix)];}

// Aliases for getAtlasSprite
inline AtlasSprite& getSprite(const std::string& nameId)                   {return getAtlasSprite(nameId);}
inline AtlasSprite& getSprite(const std::string& nameId, int suffix)       {return getAtlasSprite(nameId, suffix);}
inline AtlasSprite& getSprite(const std::string_view& nameId)              {return getAtlasSprite(nameId);}
inline AtlasSprite& getSprite(const std::string_view& nameId, int suffix)  {return getAtlasSprite(nameId, suffix);}
inline AtlasSprite& getSprite(const char* nameId)                          {return getAtlasSprite(nameId);}
inline AtlasSprite& getSprite(const char* nameId, int suffix)              {return getAtlasSprite(nameId, suffix);}

void DrawSpriteAtlas(Texture2D &atlas, AtlasSprite sprite, int x, int y, Color color);
void DrawSpriteAtlas(Texture2D &atlas, AtlasSprite sprite, int x, int y, float angle, Color color);
void DrawSpriteAtlas(Texture2D &atlas, AtlasSprite sprite, int x, int y, float xscale, float yscale, float angle, Color color);
void DrawSpriteAtlas(Texture2D &atlas, AtlasSprite sprite, ::Rectangle slice, int x, int y, float xscale, float yscale, float angle, Color color);  // warning: it ignores origin of sprite

constexpr inline std::string_view mino_16[] = {
    "16x16_I", "16x16_J", "16x16_L", "16x16_O", "16x16_S", "16x16_T", "16x16_Z",
};
constexpr inline std::string_view mino_24[] = {
    "24x24_I", "24x24_J", "24x24_L", "24x24_O", "24x24_S", "24x24_T", "24x24_Z",
};
//brick0 ~ brick6
constexpr inline std::string_view brick[] = {
    "brick0", "brick1", "brick2", "brick3", "brick4", "brick5",
};
//combo_0 1 2 3 4 5 6 7 8 9 x C o m b screamer
constexpr inline std::string_view comboIdx[] = {
    "combo_0", "combo_1", "combo_2", "combo_3", "combo_4", "combo_5", "combo_6", "combo_7", "combo_8", "combo_9", "combo_x", "combo_C", "combo_o", "combo_m", "combo_b", "combo_screamer",
};

inline void PrintSprite(AtlasSprite sprite) {
    // print every properties
    std::cout << "nameId: " << sprite.nameId << std::endl << "originX: " << sprite.originX << std::endl << "originY: " << sprite.originY << std::endl << "positionX: " << sprite.positionX << std::endl << "positionY: " << sprite.positionY << std::endl << "sourceWidth: " << sprite.sourceWidth << std::endl << "sourceHeight: " << sprite.sourceHeight << std::endl << "padding: " << sprite.padding << std::endl << "trimmed: " << sprite.trimmed << std::endl << "trimX: " << sprite.trimX << std::endl << "trimY: " << sprite.trimY << std::endl << "trimWidth: " << sprite.trimWidth << std::endl << "trimHeight: " << sprite.trimHeight << std::endl << "value: " << sprite.value << std::endl << "offsetX: " << sprite.offsetX << std::endl << "offsetY: " << sprite.offsetY << std::endl << "advanceX: " << sprite.advanceX << std::endl;
}

#endif
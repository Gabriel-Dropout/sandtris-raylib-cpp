#include <iostream>
#include <map>
#include <string_view>

#include "raylib.h"
#include "raylib-cpp.hpp"
#define RPNG_IMPLEMENTATION
#define RPNG_DEFLATE_IMPLEMENTATION
#include "rpng.h"
#include "atlas.h"

// Atlas sprite structure


// Load sprite data from .rtpb file data
static AtlasSprite *LoadAtlasSpriteData(const unsigned char *rtpbData, int rtpbSize, int *spriteCount);

// Get one sprite from the array by nameId
static AtlasSprite GetSprite(AtlasSprite *sprites, int spriteCount, const char *nameId);

raylib::Texture atlas;
std::map<std::string, AtlasSprite> spriteMap;

void initAtlas(const std::string& filename) {
    atlas = raylib::Texture(filename);

	// Read custom PNG chunk: rTPb
    int spriteCount = 0;
    AtlasSprite *spriteList = NULL;

    // NOTE: chunk.data contains a standard .rtpb binary file, we can process it
    rpng_chunk chunk = rpng_chunk_read(filename.c_str(), "rTPb");
    if(chunk.data == NULL) {
        std::cout << "Error loading atlas file" << std::endl;
        exit(1);
    }

    // Load sprite data from .rtpb file data
    spriteList = LoadAtlasSpriteData(chunk.data, chunk.length, &spriteCount);
    if(spriteList == NULL) {
        std::cout << "Error loading atlas file" << std::endl;
        exit(1);
    }

    // iterate spriteList to make a map
    for(int i = 0; i < spriteCount; i++) {
        auto tmp = spriteList[i];
        spriteMap.insert(std::make_pair(std::string_view( spriteList[i].nameId ), tmp));
    }
    // Free chunk data
    RPNG_FREE(chunk.data);
    RPNG_FREE(spriteList);
    
    std::cout << "Sprite Count: " << spriteCount << std::endl;
}


void DrawSpriteAtlas(Texture2D &atlas, AtlasSprite sprite, int x, int y, Color color) {
    DrawTexturePro(atlas,
        Rectangle{(float)sprite.positionX, (float)sprite.positionY, (float)sprite.sourceWidth, (float)sprite.sourceHeight},
        Rectangle{(float)x, (float)y, (float)sprite.sourceWidth, (float)sprite.sourceHeight},
        Vector2{(float)sprite.originX, (float)sprite.originY},
        0.f,
        color);
}
void DrawSpriteAtlas(Texture2D &atlas, AtlasSprite sprite, int x, int y, float angle, Color color) {
    DrawTexturePro(atlas,
        Rectangle{(float)sprite.positionX, (float)sprite.positionY, (float)sprite.sourceWidth, (float)sprite.sourceHeight},
        Rectangle{(float)x, (float)y, (float)sprite.sourceWidth, (float)sprite.sourceHeight},
        Vector2{(float)sprite.originX, (float)sprite.originY},
        angle,
        color);
}
void DrawSpriteAtlas(Texture2D &atlas, AtlasSprite sprite, int x, int y, float xscale, float yscale, float angle, Color color) {
    if(xscale >= 0)
        DrawTexturePro(atlas,
            Rectangle{(float)sprite.positionX, (float)sprite.positionY, (float)sprite.sourceWidth, (float)sprite.sourceHeight},
            Rectangle{(float)x, (float)y, (float)sprite.sourceWidth*xscale, (float)sprite.sourceHeight*yscale},
            Vector2{(float)sprite.originX*xscale, (float)sprite.originY*yscale},
            angle,
            color);
    else
        DrawTexturePro(atlas,
            Rectangle{(float)sprite.positionX, (float)sprite.positionY, -(float)sprite.sourceWidth, (float)sprite.sourceHeight},
            Rectangle{(float)x, (float)y, -(float)sprite.sourceWidth*xscale, (float)sprite.sourceHeight*yscale},
            Vector2{(float)sprite.originX*(-xscale), (float)sprite.originY*yscale},
            angle,
            color);
}
// warning: it ignores origin of sprite
void DrawSpriteAtlas(Texture2D &atlas, AtlasSprite sprite, ::Rectangle slice, int x, int y, float xscale, float yscale, float angle, Color color) {
    DrawTexturePro(atlas,
        Rectangle{(float)sprite.positionX + slice.x, (float)sprite.positionY + slice.y, slice.width, slice.height},
        Rectangle{(float)x, (float)y, slice.width*xscale, slice.height*yscale},
        Vector2{0, 0},
        angle,
        color);
}



// Load sprite data from .rtpb file data
static AtlasSprite *LoadAtlasSpriteData(const unsigned char *rtpbData, int rtpbSize, int *spriteCount)
{
    int count = 0;
    AtlasSprite *sprites = NULL;
    char *rtpbDataPtr = (char *)rtpbData;
    
    // rTexPacker Binary File Structure (.rtpb)
    // ------------------------------------------------------
    // Offset  | Size    | Type       | Description
    // ------------------------------------------------------
    // File header (8 bytes)
    // 0       | 4       | char       | Signature: "rTPb"
    // 4       | 2       | short      | Version: 200
    // 6       | 2       | short      | reserved

    // General info data (16 bytes)
    // 8       | 4       | int        | Sprites packed
    // 12      | 4       | int        | Flags: 0-Default, 1-Atlas image included
    // 16      | 2       | short      | Font type: 0-No font, 1-Normal, 2-SDF
    // 18      | 2       | short      | Font size
    // 20      | 2       | short      | Font SDF padding
    // 22      | 2       | short      | reserved

    // Sprites properties data
    //  - Size (only sprites): 128 + 48 bytes
    //  - Size (font sprites): 128 + 64 bytes
    // foreach (sprite.packed)
    // {
            // Default sprites data (128 + 48 bytes)
    //   ...   | 128     | char       | Sprite Name identifier
    //   ...   | 4       | int        | Sprite Origin X
    //   ...   | 4       | int        | Sprite Origin Y
    //   ...   | 4       | int        | Sprite Position X
    //   ...   | 4       | int        | Sprite Position Y
    //   ...   | 4       | int        | Sprite Source Width
    //   ...   | 4       | int        | Sprite Source Height
    //   ...   | 4       | int        | Sprite Padding
    //   ...   | 4       | int        | Sprite is trimmed?
    //   ...   | 4       | int        | Sprite Trimmed Rectangle X
    //   ...   | 4       | int        | Sprite Trimmed Rectangle Y
    //   ...   | 4       | int        | Sprite Trimmed Rectangle Width
    //   ...   | 4       | int        | Sprite Trimmed Rectangle Height
    //      if (atlas.isFont)
    //      {
            // Additional font data (16 bytes)
    //   ...   | 4       | int        | Character unicode value
    //   ...   | 4       | int        | Character offset x
    //   ...   | 4       | int        | Character offset y
    //   ...   | 4       | int        | Character advance x
    //      }
    // }
    
    // Check signature
    if ((rtpbDataPtr[0] == 'r') && (rtpbDataPtr[1] == 'T') && (rtpbDataPtr[2] == 'P') && (rtpbDataPtr[3] == 'b'))      // Valid rTPb file
    {
        rtpbDataPtr += 4;   // Move to next value
        
        int version = ((short *)rtpbDataPtr)[0];
        
        if (version == 200) // This is the rTPb version we will read
        {
            rtpbDataPtr += 2*sizeof(short);     // Skip the version and reserved values
    
            count = ((int *)rtpbDataPtr)[0];    // Number of sprites packed in the atlas
            rtpbDataPtr += sizeof(int);         // Skip sprites packed
            rtpbDataPtr += sizeof(int);         // Skip flags (0 by default, no image included)
            
            // Read font info, it could be useful
            short fontType = ((short *)rtpbDataPtr)[0];         // Font type: 0-No font, 1-Normal, 2-SDF
            short fontSize = ((short *)rtpbDataPtr)[1];         // Font size
            short fontSdfPadding = ((short *)rtpbDataPtr)[2];   // Font SDF padding
            
            rtpbDataPtr += 4*sizeof(short);     // Skip to sprites data
            
            // We can start initializing our sprites array
            // NOTE: We initialize all values to 0
            sprites = (AtlasSprite *)RL_CALLOC(count, sizeof(AtlasSprite));
            
            // Read and copy sprites data from rTPb data
            for (int i = 0; i < count; i++)
            {
                memcpy(sprites[i].nameId, rtpbDataPtr, 128);            // Sprite NameId (128 bytes by default)
                //printf("Sprite %i name id: %s\n", i, sprites[i].nameId);
                rtpbDataPtr += 128;

                memcpy(&sprites[i].originX, rtpbDataPtr, 4);             // Sprite Origin X
                memcpy(&sprites[i].originY, rtpbDataPtr + 4, 4);         // Sprite Origin Y
                memcpy(&sprites[i].positionX, rtpbDataPtr + 8, 4);       // Sprite Position X
                memcpy(&sprites[i].positionY, rtpbDataPtr + 12, 4);      // Sprite Position Y
                memcpy(&sprites[i].sourceWidth, rtpbDataPtr + 16, 4);    // Sprite Source Width
                memcpy(&sprites[i].sourceHeight, rtpbDataPtr + 20, 4);   // Sprite Source Height
                memcpy(&sprites[i].padding, rtpbDataPtr + 24, 4);        // Sprite Padding
                memcpy(&sprites[i].trimmed, rtpbDataPtr + 28, 4);        // Sprite is trimmed?
                memcpy(&sprites[i].trimX, rtpbDataPtr + 32, 4);          // Sprite Trimmed Rectangle X
                memcpy(&sprites[i].trimY, rtpbDataPtr + 36, 4);          // Sprite Trimmed Rectangle Y
                memcpy(&sprites[i].trimWidth, rtpbDataPtr + 40, 4);      // Sprite Trimmed Rectangle Width
                memcpy(&sprites[i].trimHeight, rtpbDataPtr + 44, 4);     // Sprite Trimmed Rectangle Height
                rtpbDataPtr += 48;
                
                if (fontType > 0)
                {
                    memcpy(&sprites[i].value, rtpbDataPtr, 4);           // Character value (Unicode)
                    memcpy(&sprites[i].offsetX, rtpbDataPtr + 4, 4);     // Character offset X when drawing
                    memcpy(&sprites[i].offsetY, rtpbDataPtr + 8, 4);     // Character offset Y when drawing
                    memcpy(&sprites[i].advanceX, rtpbDataPtr + 12, 4);   // Character advance position X
                    rtpbDataPtr += 16;
                }
            }
        }
    }
    
    *spriteCount = count;
    return sprites;
}

// Get one sprite from the array by nameId
static AtlasSprite GetSprite(AtlasSprite *sprites, int spriteCount, const char *nameId)
{
    AtlasSprite sprite = { 0 };
    
    for (int i = 0; i < spriteCount; i++)
    {
        if (TextIsEqual(nameId, sprites[i].nameId))
        {
            sprite = sprites[i];
            break;
        }
    }

    return sprite;
}
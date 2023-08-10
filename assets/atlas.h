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

#define ATLAS_ATLAS_IMAGE_PATH      "atlas.png"
#define ATLAS_ATLAS_SPRITE_COUNT    47

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
static rtpAtlasSprite rtpDescAtlas[47] = {
    { "16x16_I", 8, 8, 448, 120, 16, 16, 0, false, 0, 6, 16, 4 },
    { "16x16_J", 8, 8, 432, 120, 16, 16, 0, false, 2, 4, 12, 8 },
    { "16x16_L", 8, 8, 480, 104, 16, 16, 0, false, 2, 4, 12, 8 },
    { "16x16_O", 8, 8, 480, 88, 16, 16, 0, false, 4, 4, 8, 8 },
    { "16x16_S", 8, 8, 480, 72, 16, 16, 0, false, 2, 4, 12, 8 },
    { "16x16_T", 8, 8, 464, 120, 16, 16, 0, false, 2, 4, 12, 8 },
    { "16x16_Z", 8, 8, 480, 120, 16, 16, 0, false, 2, 4, 12, 8 },
    { "24x24_I", 12, 12, 456, 96, 24, 24, 0, false, 0, 9, 24, 6 },
    { "24x24_J", 12, 12, 432, 96, 24, 24, 0, false, 3, 6, 18, 12 },
    { "24x24_L", 12, 12, 408, 96, 24, 24, 0, false, 3, 6, 18, 12 },
    { "24x24_O", 12, 12, 432, 72, 24, 24, 0, false, 6, 6, 12, 12 },
    { "24x24_S", 12, 12, 408, 72, 24, 24, 0, false, 3, 6, 18, 12 },
    { "24x24_T", 12, 12, 408, 120, 24, 24, 0, false, 3, 6, 18, 12 },
    { "24x24_Z", 12, 12, 456, 72, 24, 24, 0, false, 3, 6, 18, 12 },
    { "brick5", 0, 0, 353, 128, 9, 9, 0, false, 0, 0, 9, 9 },
    { "brick0", 0, 0, 344, 128, 9, 9, 0, false, 0, 0, 9, 9 },
    { "brick1", 0, 0, 335, 128, 9, 9, 0, false, 0, 0, 9, 9 },
    { "brick2", 0, 0, 326, 128, 9, 9, 0, false, 0, 0, 9, 9 },
    { "brick3", 0, 0, 362, 128, 9, 9, 0, false, 0, 0, 9, 9 },
    { "brick4", 0, 0, 371, 128, 9, 9, 0, false, 0, 0, 9, 9 },
    { "combo_x", 0, 0, 307, 128, 10, 11, 0, false, 0, 0, 10, 11 },
    { "combo_0", 0, 0, 498, 92, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_1", 0, 0, 289, 128, 6, 12, 0, false, 0, 0, 6, 12 },
    { "combo_2", 0, 0, 280, 128, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_3", 0, 0, 498, 104, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_4", 0, 0, 498, 116, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_5", 0, 0, 498, 80, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_6", 0, 0, 498, 68, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_7", 0, 0, 498, 56, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_8", 0, 0, 500, 32, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_9", 0, 0, 500, 44, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_b", 0, 0, 496, 16, 9, 16, 0, false, 0, 0, 9, 16 },
    { "combo_C", 0, 0, 496, 0, 11, 16, 0, false, 0, 0, 11, 16 },
    { "combo_m", 0, 0, 295, 128, 12, 11, 0, false, 0, 0, 12, 11 },
    { "combo_o", 0, 0, 317, 128, 9, 11, 0, false, 0, 0, 9, 11 },
    { "combo_screamer", 0, 0, 496, 32, 4, 16, 0, false, 0, 0, 4, 16 },
    { "sandtetris1", 28, 34, 140, 0, 140, 198, 0, false, 0, 0, 140, 198 },
    { "sandtetris2", 28, 34, 0, 0, 140, 198, 0, false, 0, 0, 140, 198 },
    { "logo", 64, 64, 280, 0, 128, 128, 0, false, 4, 10, 119, 108 },
    { "wizard_angry", 13, 36, 408, 0, 22, 36, 0, false, 0, 0, 22, 36 },
    { "wizard_idle", 13, 36, 430, 0, 22, 36, 0, false, 0, 3, 21, 33 },
    { "wizard_walk1", 13, 36, 452, 0, 22, 36, 0, false, 0, 4, 21, 32 },
    { "wizard_walk2", 13, 36, 474, 0, 22, 36, 0, false, 1, 4, 21, 32 },
    { "wizard_walk3", 13, 36, 408, 36, 22, 36, 0, false, 2, 3, 20, 33 },
    { "wizard_walk4", 13, 36, 430, 36, 22, 36, 0, false, 3, 3, 18, 33 },
    { "wizard_walk5", 13, 36, 452, 36, 22, 36, 0, false, 2, 3, 18, 33 },
    { "wizard_walk6", 13, 36, 474, 36, 22, 36, 0, false, 1, 4, 19, 32 },
};

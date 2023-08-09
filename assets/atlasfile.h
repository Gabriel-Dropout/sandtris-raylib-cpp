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

#define ATLAS_ATLASFILE_IMAGE_PATH      "atlasfile.png"
#define ATLAS_ATLASFILE_SPRITE_COUNT    38

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
static rtpAtlasSprite rtpDescAtlasfile[38] = {
    { "16x16_I", 8, 8, 280, 24, 16, 16, 0, false, 0, 6, 16, 4 },
    { "16x16_J", 8, 8, 480, 16, 16, 16, 0, false, 2, 4, 12, 8 },
    { "16x16_L", 8, 8, 464, 16, 16, 16, 0, false, 2, 4, 12, 8 },
    { "16x16_O", 8, 8, 448, 16, 16, 16, 0, false, 4, 4, 8, 8 },
    { "16x16_S", 8, 8, 480, 0, 16, 16, 0, false, 2, 4, 12, 8 },
    { "16x16_T", 8, 8, 464, 0, 16, 16, 0, false, 2, 4, 12, 8 },
    { "16x16_Z", 8, 8, 448, 0, 16, 16, 0, false, 2, 4, 12, 8 },
    { "24x24_I", 12, 12, 424, 0, 24, 24, 0, false, 0, 9, 24, 6 },
    { "24x24_J", 12, 12, 280, 0, 24, 24, 0, false, 3, 6, 18, 12 },
    { "24x24_L", 12, 12, 400, 0, 24, 24, 0, false, 3, 6, 18, 12 },
    { "24x24_O", 12, 12, 376, 0, 24, 24, 0, false, 6, 6, 12, 12 },
    { "24x24_S", 12, 12, 352, 0, 24, 24, 0, false, 3, 6, 18, 12 },
    { "24x24_T", 12, 12, 328, 0, 24, 24, 0, false, 3, 6, 18, 12 },
    { "24x24_Z", 12, 12, 304, 0, 24, 24, 0, false, 3, 6, 18, 12 },
    { "brick5", 0, 0, 466, 32, 9, 9, 0, false, 0, 0, 9, 9 },
    { "brick0", 0, 0, 448, 32, 9, 9, 0, false, 0, 0, 9, 9 },
    { "brick1", 0, 0, 457, 32, 9, 9, 0, false, 0, 0, 9, 9 },
    { "brick2", 0, 0, 418, 24, 9, 9, 0, false, 0, 0, 9, 9 },
    { "brick3", 0, 0, 427, 24, 9, 9, 0, false, 0, 0, 9, 9 },
    { "brick4", 0, 0, 475, 32, 9, 9, 0, false, 0, 0, 9, 9 },
    { "combo_x", 0, 0, 399, 24, 10, 11, 0, false, 0, 0, 10, 11 },
    { "combo_0", 0, 0, 318, 24, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_1", 0, 0, 381, 24, 6, 12, 0, false, 0, 0, 6, 12 },
    { "combo_2", 0, 0, 336, 24, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_3", 0, 0, 345, 24, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_4", 0, 0, 354, 24, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_5", 0, 0, 363, 24, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_6", 0, 0, 372, 24, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_7", 0, 0, 327, 24, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_8", 0, 0, 309, 24, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_9", 0, 0, 300, 24, 9, 12, 0, false, 0, 0, 9, 12 },
    { "combo_b", 0, 0, 496, 16, 9, 16, 0, false, 0, 0, 9, 16 },
    { "combo_C", 0, 0, 496, 0, 11, 16, 0, false, 0, 0, 11, 16 },
    { "combo_m", 0, 0, 387, 24, 12, 11, 0, false, 0, 0, 12, 11 },
    { "combo_o", 0, 0, 409, 24, 9, 11, 0, false, 0, 0, 9, 11 },
    { "combo_screamer", 0, 0, 296, 24, 4, 16, 0, false, 0, 0, 4, 16 },
    { "sandtetris1", 28, 34, 140, 0, 140, 198, 0, false, 0, 0, 140, 198 },
    { "sandtetris2", 28, 34, 0, 0, 140, 198, 0, false, 0, 0, 140, 198 },
};

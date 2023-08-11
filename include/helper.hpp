#ifndef HELPER_HPP
#define HELPER_HPP

#include <algorithm>
#include <string>
#include <vector>

#include "raylib.h"
#include "raylib-cpp.hpp"
#include "raymath.hpp"

#include "atlas.h"

inline Color colLighter(Color color, int amount) {
    return Color{
        (unsigned char) std::clamp(color.r + (int)amount, 0, 255),
        (unsigned char) std::clamp(color.g + (int)(amount*1.3), 0, 255),
        (unsigned char) std::clamp(color.b + (int)(amount*1.5), 0, 255),
        255
    };
}
inline double easeInOutCubic(double x) {
    return x < 0.5 ? 4 * x * x * x : 1 - std::pow(-2 * x + 2, 3) / 2;
}
inline float step(float x, float freezeRatio, int level) {
    x *= level;
    int curlevel = (int)(x);
    x -= curlevel;
    if(x >= 1 - freezeRatio) return (float)(curlevel + 1);
    return curlevel + easeInOutCubic(x/(1 - freezeRatio + 1e-15));
}

inline void drawCombo(int combo, int x, int y, int scale, int halign, int valign, float amount = 0.0f, int timer = 0) {
    if(combo < 0) return;
    int idx_c = 11, idx_o = 12, idx_m = 13, idx_b = 14, idx_sc = 15, idx_x = 10, idx_space = -1;

    // generate index list to draw
    std::vector<int> idxList;
    idxList.push_back(idx_x);
    while(combo>0) {
        int tmp = combo%10;
        combo /= 10;
        idxList.insert(idxList.begin() + 1, tmp);
        //idxList.push_back(tmp);
    }
    idxList.insert(idxList.end(), {idx_space, idx_c, idx_o, idx_m, idx_b, idx_o, idx_sc});

    // calculate text width for alignment
    int textWidth = 0;
    for(int idx : idxList) {
        if(idx == idx_space) textWidth += 4*scale;
        else textWidth += (getSprite(comboIdx[idx]).sourceWidth + 1)*scale;
    }
    x -= halign*textWidth/2;
    int fillWidth = (int)(textWidth*amount);
    int xoffset = 0;

    // draw
    for(int i=0; i<idxList.size(); i++) {
        int idx = idxList[i];

        if(idx == idx_space) {
            xoffset += 4*scale;
            continue;
        }

        int drawX = x + xoffset;
        int drawY = y - valign*getSprite(comboIdx[idx]).sourceHeight*scale/2;
        drawY += sinf((timer*10 + i*20)*DEG2RAD)*scale*3;

        // fill
        if(fillWidth < xoffset)
            DrawSpriteAtlas(atlas, getSprite(comboIdx[idx]), drawX, drawY, scale, scale, 0, DARKGRAY);
        else if(fillWidth > xoffset + getSprite(comboIdx[idx]).sourceWidth*scale)
            DrawSpriteAtlas(atlas, getSprite(comboIdx[idx]), drawX, drawY, scale, scale, 0, WHITE);
        else {
            int tmpWidth = fillWidth - xoffset;
            DrawSpriteAtlas(atlas, getSprite(comboIdx[idx]), drawX, drawY, scale, scale, 0, DARKGRAY);
            DrawSpriteAtlas(atlas, getSprite(comboIdx[idx]), ::Rectangle{0, 0, (float)tmpWidth/scale, (float)getSprite(comboIdx[idx]).sourceHeight}, drawX, drawY, scale, scale, 0, WHITE);
        }

        xoffset += (getSprite(comboIdx[idx]).sourceWidth + 1)*scale;
    }
}

#endif
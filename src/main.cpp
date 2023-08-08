#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <string.h>

#include "Functions.hpp"
#include "Mouse.hpp"
#include "Rectangle.hpp"
#include "RenderTexture.hpp"
#include "Texture.hpp"
#include "Vector2.hpp"
#include "raylib-cpp.hpp"
#include "raylib.h"
#include "raymath.hpp"

#include "table.hpp"
#include "mino.hpp"
#include "helper.hpp"
#include "atlas.h"
#include "gamelogic.hpp"

#define W 90
#define H 160

int globalTimer = 0;


const int BlkW = 4;
const int screenWidth = 1080;
const int screenHeight = 800;
const int TblX = screenWidth/2, TblY = screenHeight/2 + 70;

const int TblULX = TblX - BlkW*W/2,
          TblULY = TblY - BlkW*H/2;
const raylib::Vector2 TblULV = {TblULX, TblULY};


// raylib::Color cmap[6] = {WHITE, RED, BLUE, GREEN, VIOLET, DARKBROWN};


int main() {
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "Sand Tetris Demo");

    SetTargetFPS(60);

    GameLogic gameLogic(W, H);

    raylib::Texture atlas("assets/atlas.png");

    raylib::Vector2 nextOffset[] = {
        {94+1+6, 1+6},
        {94+1+6, 1+6+15},
        {94+1+6, 1+6+30},
        {94+1+6, 1+6+45},
    };
    raylib::Vector2 holdOffset(-24+2+8, 6+2+8);
    for(int i=0; i<4; i++) nextOffset[i] *= BlkW;
    holdOffset *= BlkW;
    

    InitAudioDevice();              // Initialize audio device

    // Music music = LoadMusicStream("assets/game_bgm.mp3");

    //PlayMusicStream(music);

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        gameLogic.update();
        globalTimer++;

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(raylib::Color(0xFFF0F5FF));

            DrawSpriteAtlas(atlas, mainUI[0], TblULX, TblULY, BlkW, BlkW, 0, WHITE);

            DrawTexturePro(gameLogic.getTexture(), (Rectangle){ 0, 0, W, -H},
                (Rectangle){ TblX, TblY, W*BlkW, H*BlkW },
                (Vector2){ (float)W*BlkW/2, (float)H*BlkW/2,}, 0.0f, WHITE);

            if(gameLogic.getFlag("MATCH")) {
                std::cout<<"MATCH"<<std::endl;
                for(int i=0; i<H; i++) for(int j=0; j<W; j++) {
                    if(gameLogic.isPositionMatched(j, i)) {
                        raylib::Vector2 UL(TblULX + BlkW*j, TblULY + BlkW*i);
                        raylib::Vector2 UR = UL.Add({BlkW, 0}), BL = UL.Add({0, BlkW}), BR = UL.Add({BlkW, BlkW});

                        float timer1 = (globalTimer % 30) / 30.0f;
                        float timer2 = ((globalTimer + 15) % 30) / 30.0f;

                        auto movingline = [](raylib::Vector2 v1, raylib::Vector2 v2, float timer) {
                            float v = step(timer, .0f, 2);
                            auto v3 = v1.MoveTowards(v2, std::max(BlkW*(v-1), 0.0f) - 2.0f);
                            auto v4 = v1.MoveTowards(v2, std::min(BlkW*v, 8.0f) + 2.0f);
                            DrawLineEx(v3, v4, 4, BLACK);
                        };

                        if(!gameLogic.isPositionMatched(j-1, i))
                            movingline(UL, BL, ((i+j)%2)?timer1:timer2);
                        if(!gameLogic.isPositionMatched(j+1, i))
                            movingline(BR, UR, ((i+j)%2)?timer1:timer2);
                        if(!gameLogic.isPositionMatched(j, i-1))
                            movingline(UR, UL, ((i+j+1)%2)?timer1:timer2);
                        if(!gameLogic.isPositionMatched(j, i+1))
                            movingline(BL, BR, ((i+j+1)%2)?timer1:timer2);
                    }
                }
            }

            for(int i=0; i<4; i++) for(int j=0; j<4; j++) {
                const BlockState& curBlock = gameLogic.curBlock;
                if(getMino(curBlock, i, j)) {
                    DrawRectangle(TblULX + BlkW*(curBlock.x + 9*i), TblULY + BlkW*(curBlock.y + 9*j), BlkW*9, BlkW*9, colLighter(cmap[curBlock.col], -5));
                    DrawRectangle(TblULX + BlkW*(curBlock.x + 9*i +1), TblULY + BlkW*(curBlock.y + 9*j +1), BlkW*7, BlkW*7, colLighter(cmap[curBlock.col], 25));
                    DrawRectangle(TblULX + BlkW*(curBlock.x + 9*i +2), TblULY + BlkW*(curBlock.y + 9*j +2), BlkW*5, BlkW*5, colLighter(cmap[curBlock.col], -5));
                }
            }

            int tBlkW = 3;
            for(int n=0; n<4; n++) {
                const BlockState& qBlock = gameLogic.blockQueue[n+1];
                DrawSpriteAtlas(atlas, mino_24[qBlock.type], TblULX + nextOffset[n].x, TblULY + nextOffset[n].y, BlkW/2, BlkW/2, 0, cmap[qBlock.col]);
            }

            const BlockState& holdBlock = gameLogic.holdBlock;
            if(holdBlock.type != -1) {
                DrawSpriteAtlas(atlas, mino_16[holdBlock.type], TblULX + holdOffset.x, TblULY + holdOffset.y, BlkW, BlkW, globalTimer, cmap[holdBlock.col]);
            }



        EndDrawing();
        //----------------------------------------------------------------------------------
    }

}
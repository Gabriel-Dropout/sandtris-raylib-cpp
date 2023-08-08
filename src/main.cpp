#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <string.h>

#include "Functions.hpp"
#include "Image.hpp"
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
int shockTimer = 0;
int shockMode = 0;  // 0:soft, 1:normal, 2:power


const int BlkW = 4;
const int screenWidth = 1080;
const int screenHeight = 900;
const int TblX = screenWidth/2, TblY = screenHeight/2 + 30;

int TblULX = TblX - BlkW*W/2,
    TblULY = TblY - BlkW*H/2;
const int TblULY_init = TblULY;

raylib::Image brickImages[6];


int main() {
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "Sand Tetris Demo");

    SetTargetFPS(60);

    GameLogic gameLogic(W, H);

    raylib::Texture atlas("assets/atlas.png");
    brickImages[0] = raylib::Image("assets/brick0.png");
    brickImages[1] = raylib::Image("assets/brick1.png");
    brickImages[2] = raylib::Image("assets/brick2.png");
    brickImages[3] = raylib::Image("assets/brick3.png");
    brickImages[4] = raylib::Image("assets/brick4.png");
    brickImages[5] = raylib::Image("assets/brick5.png");

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
        if(gameLogic.consumeFlag("GAMEOVER")) {
            gameLogic.reset();
        }
        if(gameLogic.consumeFlag("SOFTSHOCK")) {
            shockMode = 0;
            shockTimer = 5;
        }
        if(gameLogic.consumeFlag("SHOCK")) {
            shockMode = 1;
            shockTimer = 8;
        }
        if(gameLogic.consumeFlag("POWERSHOCK")) {
            shockMode = 2;
            shockTimer = 10;
        }
        shockTimer = std::max(shockTimer-1, 0);
        switch(shockMode) {
        case 0:
            TblULY = TblULY_init + shockTimer*(5-shockTimer);
            break;
        case 1:
            TblULY = TblULY_init + shockTimer*(8-shockTimer);
            break;
        case 2:
            TblULY = TblULY_init + shockTimer*(10-shockTimer);
            break;
        }
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(raylib::Color(0xFFF0F5FF));

            if(globalTimer/60 % 2)
                DrawSpriteAtlas(atlas, mainUI[0], TblULX, TblULY, BlkW, BlkW, 0, WHITE);
            else
                DrawSpriteAtlas(atlas, mainUI[1], TblULX, TblULY, BlkW, BlkW, 0, WHITE);

            DrawTexturePro(gameLogic.getTexture(), (Rectangle){ 0, 0, W, -H},
                (Rectangle){ (float)TblULX, (float)TblULY, W*BlkW, H*BlkW },
                (Vector2){0, 0}, 0.0f, WHITE);

            if(gameLogic.getFlag("MATCH")) {
                std::cout<<"MATCH"<<std::endl;
                for(int y=0; y<H; y++) for(int x=0; x<W; x++) {
                    if(gameLogic.isPositionMatched(x, y) && (globalTimer/20 % 2)) {
                        DrawRectangle(TblULX + BlkW*x, TblULY + BlkW*y, BlkW, BlkW, WHITE);
                    }
                }
            }

            // Draw Minos
            for(int i=0; i<4; i++) for(int j=0; j<4; j++) {
                const BlockState& curBlock = gameLogic.curBlock;
                if(getMino(curBlock, i, j)) {
                    DrawSpriteAtlas(atlas, brick[curBlock.col], TblULX + BlkW*(curBlock.x + 9*i), TblULY + BlkW*(curBlock.y + 9*j), BlkW, BlkW, 0, WHITE);
                }
            }

            for(int n=0; n<4; n++) {
                const BlockState& qBlock = gameLogic.blockQueue[n+1];
                DrawSpriteAtlas(atlas, mino_24[qBlock.type], TblULX + nextOffset[n].x, TblULY + nextOffset[n].y, BlkW/2, BlkW/2, 0, brickImages[qBlock.col].GetColor(0,0));
            }

            const BlockState& holdBlock = gameLogic.holdBlock;
            if(holdBlock.type != -1) {
                DrawSpriteAtlas(atlas, mino_16[holdBlock.type], TblULX + holdOffset.x, TblULY + holdOffset.y, BlkW, BlkW, globalTimer, brickImages[holdBlock.col].GetColor(0,0));
            }



        EndDrawing();
        //----------------------------------------------------------------------------------
    }

}
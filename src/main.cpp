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

raylib::Texture atlas;

int globalTimer = 0;
int shockTimer = 0;
int shockMode = 0;  // 0:soft, 1:normal, 2:power


const int BlkW = 4;
const int screenWidth = 700;
const int screenHeight = 900;
const int TblX = screenWidth/2, TblY = screenHeight/2 + 30;

int TblULX = TblX - BlkW*W/2,
    TblULY = TblY - BlkW*H/2;
const int TblULY_init = TblULY;

raylib::Image brickImages[6];

enum class GameState {
    MENU, GAME, GAMEOVER
};
GameState gameState = GameState::MENU;

// Main Menu things
float easeOutCubic(float t) {
    if(t<0) return 0;
    if(t>1) return 1;
    return 1 - (1-t)*(1-t)*(1-t);
}

// Game Over things
int lastScore = 0;

// Wizard things
int bombTimer = 0;
int wizardOffset = 0;


int main() {
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "Sand Tetris Demo");

    SetTargetFPS(60);

    GameLogic gameLogic(W, H);

    atlas = raylib::Texture("assets/atlas.png");
    brickImages[0] = raylib::Image("assets/individual/brick0.png");
    brickImages[1] = raylib::Image("assets/individual/brick1.png");
    brickImages[2] = raylib::Image("assets/individual/brick2.png");
    brickImages[3] = raylib::Image("assets/individual/brick3.png");
    brickImages[4] = raylib::Image("assets/individual/brick4.png");
    brickImages[5] = raylib::Image("assets/individual/brick5.png");

    raylib::Vector2 nextOffset[] = {
        {94+1+6, 1+6},
        {94+1+6, 1+6+15},
        {94+1+6, 1+6+30},
        {94+1+6, 1+6+45},
    };
    raylib::Vector2 holdOffset(-24+2+8, 6+2+8);
    raylib::Vector2 scoreOffset(3 - 28, 16 - 34 + 5);
    for(int i=0; i<4; i++) nextOffset[i] *= BlkW;
    holdOffset *= BlkW;
    scoreOffset *= BlkW;
    

    InitAudioDevice();              // Initialize audio device

    raylib::Music music("assets/game_bgm.mp3");
    raylib::Sound hitfx("assets/hit.wav");
    raylib::Sound matchfx("assets/match.wav");
    raylib::Sound bombfx("assets/bomb.wav");
    
    PlayMusicStream(music);

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) {
        UpdateMusicStream(music);
        globalTimer++;
        bombTimer = std::max(bombTimer-1, 0);

        switch(gameState) {
        case GameState::MENU:
            if(IsKeyPressed(KEY_ENTER)) {
                gameState = GameState::GAME;
            }
            break;
        case GameState::GAME:
            // Update
            gameLogic.update();
            if(gameLogic.consumeFlag("GAMEOVER")) {
                lastScore = gameLogic.getScore();
                gameLogic.reset();
                gameState = GameState::GAMEOVER;
            }
            if(gameLogic.consumeFlag("SOFTSHOCK")) {
                shockMode = 0;
                shockTimer = 5;
                hitfx.Play();
            }
            if(gameLogic.consumeFlag("SHOCK")) {
                shockMode = 1;
                shockTimer = 8;
                hitfx.Play();
            }
            if(gameLogic.consumeFlag("POWERSHOCK")) {
                shockMode = 2;
                shockTimer = 10;
                hitfx.Play();
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

            // Wizard magic
            if(gameLogic.consumeFlag("BOMB")) {
                bombTimer = 60;
                bombfx.Play();
            }
            break;
        case GameState::GAMEOVER:
            if(IsKeyPressed(KEY_ENTER)) {
                gameState = GameState::GAME;
            }
            break;
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(raylib::Color(0xFFF0F5FF));

            switch(gameState) {
            case GameState::MENU:{
                auto *starttext = "Press Enter to start";
                auto textsize = MeasureTextEx(GetFontDefault(), starttext, 20, (int)(20/10));
                if(globalTimer/30 % 2)
                    DrawText(starttext, (int)(screenWidth/2) - textsize.x/2, 700, 20, BLACK);
                DrawSpriteAtlas(atlas, logo, screenWidth/2, 300 - 526*(1-easeOutCubic((float)globalTimer/90)), BlkW, BlkW, 0, WHITE);
                break;
            }
            case GameState::GAME:{
                if(globalTimer/60 % 2)
                    DrawSpriteAtlas(atlas, mainUI[0], TblULX, TblULY, BlkW, BlkW, 0, WHITE);
                else
                    DrawSpriteAtlas(atlas, mainUI[1], TblULX, TblULY, BlkW, BlkW, 0, WHITE);

                DrawTexturePro(gameLogic.getTexture(), (Rectangle){ 0, 0, W, -H},
                    (Rectangle){ (float)TblULX, (float)TblULY, W*BlkW, H*BlkW },
                    (Vector2){0, 0}, 0.0f, WHITE);
                
                // Bomb Effect
                if(bombTimer>0) {
                    int r=15;
                    for(int i=-r; i<=r; i++) for(int j=-r; j<=r; j++) {
                        float inner = (1.0f - bombTimer/60.0f)*r;
                        if(i*i+j*j>r*r) continue;
                        if(i*i+j*j<inner*inner) continue;

                        DrawRectangle(TblULX + BlkW*(gameLogic.bombx+i), TblULY + BlkW*(gameLogic.bomby+j), BlkW, BlkW, WHITE);
                    }
                }

                if(gameLogic.getFlag("MATCH")) {
                    for(int y=0; y<H; y++) for(int x=0; x<W; x++) {
                        if(gameLogic.isPositionMatched(x, y) && (globalTimer/20 % 2)) {
                            DrawRectangle(TblULX + BlkW*x, TblULY + BlkW*y, BlkW, BlkW, WHITE);
                        }
                    }
                }
                if(gameLogic.consumeFlag("MATCH_ONCE")) {
                    matchfx.Play();
                }

                // Draw Minos
                for(int i=0; i<4; i++) for(int j=0; j<4; j++) {
                    const BlockState& curBlock = gameLogic.curBlock;
                    float realx = gameLogic.realx;  // for smooth movement, not use curBlock.x
                    if(getMino(curBlock, i, j)) {
                        DrawSpriteAtlas(atlas, brick[curBlock.col], TblULX + BlkW*(realx + 9*i), TblULY + BlkW*(curBlock.y + 9*j), BlkW, BlkW, 0, WHITE);
                    }
                }

                for(int n=0; n<4; n++) {
                    const BlockState& qBlock = gameLogic.blockQueue[n+1];
                    DrawSpriteAtlas(atlas, mino_24[qBlock.type], TblULX + nextOffset[n].x, TblULY + nextOffset[n].y, (int)(BlkW/2), (int)(BlkW/2), 0, brickImages[qBlock.col].GetColor(0,0));
                }

                const BlockState& holdBlock = gameLogic.holdBlock;
                if(holdBlock.type != -1) {
                    DrawSpriteAtlas(atlas, mino_16[holdBlock.type], TblULX + holdOffset.x, TblULY + holdOffset.y, BlkW, BlkW, globalTimer, brickImages[holdBlock.col].GetColor(0,0));
                }

                // Draw Score
                auto *scoretext = TextFormat("Score: %d", gameLogic.getScore());
                auto scoresize = MeasureTextEx(GetFontDefault(), scoretext, 20, (int)(20/10));
                DrawText(scoretext, TblULX + scoreOffset.x + 10, TblULY + scoreOffset.y - scoresize.y/2, 20, BLACK);

                // Draw Combo
                if(gameLogic.getCombo() > 0)
                    drawCombo(gameLogic.getCombo(), TblX, TblULY + 100, 3, 1, 2, (float)gameLogic.getComboTimer()/gameLogic.comboTimerMax, globalTimer);
                
                // Draw Wizard
                rtpAtlasSprite wizardCurImg;
                wizardOffset *= 0.9;
                switch(gameLogic.wizardState) {
                case GameLogic::WS_IDLE:
                    wizardCurImg = wizardIdleImg;
                    break;
                case GameLogic::WS_MOVE:
                    wizardCurImg = wizardWalkImg[(globalTimer/10) % 6];
                    break;
                case GameLogic::WS_ANGRY:
                    wizardOffset = sinf((float)(60 - bombTimer)/20.0f)*20.0f;
                    wizardCurImg = wizardAngryImg;
                    break;
                }
                DrawSpriteAtlas(atlas, wizardCurImg, TblULX + BlkW*gameLogic.wizardX + wizardOffset, TblULY + BlkW, BlkW*gameLogic.wizardDir, BlkW, 0, WHITE);
                DrawText(TextFormat("%d", gameLogic.wizardState), 10, 0, 20, BLACK);
                DrawText(TextFormat("%d", gameLogic.wizardX), 10, 60, 20, BLACK);
                break;
            }
            case GameState::GAMEOVER:{
                auto *gameovertext = "GAME OVER";
                auto textsize = MeasureTextEx(GetFontDefault(), gameovertext, 40, (int)(40/10));
                DrawText(gameovertext, (int)(screenWidth/2) - textsize.x/2, 350, 40, BLACK);

                auto *lastscoretext = TextFormat("Score: %d", lastScore);
                textsize = MeasureTextEx(GetFontDefault(), lastscoretext, 30, (int)(30/10));
                DrawText(lastscoretext, (int)(screenWidth/2) - textsize.x/2, 350 + 60, 30, BLACK);

                auto *restarttext = "PRESS ENTER TO RESTART";
                textsize = MeasureTextEx(GetFontDefault(), restarttext, 30, (int)(30/10));
                if(globalTimer/30 % 2)
                    DrawText(restarttext, (int)(screenWidth/2) - textsize.x/2, 350 + 60 + 45, 30, BLACK);
                break;
            }
            }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

}
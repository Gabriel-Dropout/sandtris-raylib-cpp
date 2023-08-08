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

#define W 90
#define H 160

int globalTimer = 0;


const int BlkW = 6;
const int TblX = 800, TblY = 690;
const int screenWidth = 1600;
const int screenHeight = 1200;

const int TblULX = TblX - BlkW*W/2,
          TblULY = TblY - BlkW*H/2;
const raylib::Vector2 TblULV = {TblULX, TblULY};


raylib::Color cmap[6] = {WHITE, RED, BLUE, GREEN, VIOLET, DARKBROWN};


bool clearing = false;
int clearTimer = -1;
const int clearTimerMax = 90;


BlockState blockQueue[5];
BlockState& curBlock = blockQueue[0];
BlockState holdBlock = {40, 0, 0, -1, 0};

void popBlock() {
    for(int i=0; i<4; i++)
        blockQueue[i] = blockQueue[i+1];
    blockQueue[4] = {40, 0, 0, GetRandomValue(0, 6), GetRandomValue(1, 5)};
}

bool colCheck(const Table& table, const BlockState& blk) {
    for(int i=0; i<4; i++) for(int j=0; j<4; j++) {
        if(!getMino(blk, i, j)) continue;
        for(int a=0; a<9; a++) for(int b=0; b<9; b++) {
            int tmpx = blk.x + i*9 + a, tmpy = blk.y + j*9 + b;
            if(!table.isSafe(tmpx, tmpy) || table.getTableColor(tmpx, tmpy)!=0) return true;
        }
    }
    return false;
}
bool colCheckWall(const Table& table, const BlockState& blk) {
    for(int i=0; i<4; i++) for(int j=0; j<4; j++) {
        if(!getMino(blk, i, j)) continue;
        for(int a=0; a<9; a++) for(int b=0; b<9; b++) {
            int tmpx = blk.x + i*9 + a, tmpy = blk.y + j*9 + b;
            if(!table.isSafe(tmpx, tmpy)) return true;
        }
    }
    return false;
}

void putBlock(Table& table, const BlockState& blk) {
    for(int i=0; i<4; i++) for(int j=0; j<4; j++) {
        if(!getMino(blk, i, j)) continue;
        for(int a=0; a<9; a++) for(int b=0; b<9; b++) {
            int tmpx = blk.x + i*9 + a, tmpy = blk.y + j*9 + b;
            if(a!=0 && a!=8 && b!=0 && b!=8 && (a==1 || a==7 || b==1 | b==7))
                table.setTableEntry(tmpx, tmpy, {blk.col, 0, colLighter(cmap[blk.col], 25)});
            else
                table.setTableEntry(tmpx, tmpy, {blk.col, 0, colLighter(cmap[blk.col], -5)});
        }
    }
}

bool resolveCollision(const Table& table, BlockState& blk) {
    while(colCheckWall(table, blk)) {
        for(int i=0; i<4; i++) for(int j=0; j<4; j++) {
            if(!getMino(blk, i, j)) continue;
            for(int a=0; a<9; a++) for(int b=0; b<9; b++) {
                int tmpx = blk.x + i*9 + a, tmpy = blk.y + j*9 + b;
                if(!table.isSafe(tmpx, tmpy)) {
                    if(tmpx<0) {
                        blk.x++;
                    } else if(tmpx>=W) {
                        blk.x--;
                    } else if(tmpy>=H) {
                        blk.y--;
                    } else {
                        std::cout<<"ERROR OCCURED. CANNOT RESOLVE COLLISION." << std::endl;
                        exit(1);
                    }
                }
            }
        }
    }
    while(colCheck(table, blk)) {
        blk.y--;
    }

    return false;
}


int main() {
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "Sand Tetris Demo");

    SetTargetFPS(60);

    Table table = Table(W, H);
    popBlock();
    popBlock();
    popBlock();
    popBlock();
    popBlock();


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
        globalTimer++;
        //UpdateMusicStream(music);   // Update music buffer with new stream data
        // Update
        //----------------------------------------------------------------------------------
        clearTimer = std::max(clearTimer-1, -1);

        table.setAutoTexture();
        if(clearTimer == -1) {

            if(IsKeyPressed(KEY_F)) {
                if(holdBlock.type != -1) {
                    std::swap(curBlock, holdBlock);
                    curBlock.x = holdBlock.x;
                    curBlock.y = holdBlock.y;
                    resolveCollision(table, curBlock);
                } else {
                    holdBlock = curBlock;
                    popBlock();
                }
            }

            if(IsKeyDown(KEY_RIGHT)) {
                BlockState tmp = curBlock;
                tmp.x += 1;
                if(!colCheck(table, tmp)) {
                    curBlock = tmp;
                }
            }
            if(IsKeyDown(KEY_LEFT)) {
                BlockState tmp = curBlock;
                tmp.x -= 1;
                if(!colCheck(table, tmp)) {
                    curBlock = tmp;
                }
            }
            if(IsKeyPressed(KEY_UP)) {
                curBlock.rot = (curBlock.rot+1)%4;
                resolveCollision(table, curBlock);
            }
            int yspd = IsKeyDown(KEY_DOWN) ? 3 : 1;
            for(int i=0; i<yspd; i++) {
                BlockState tmp = curBlock;
                tmp.y += 1;
                if(!colCheck(table, tmp)) {
                    curBlock = tmp;
                } else {
                    putBlock(table, curBlock);
                    popBlock();
                    break;
                }
            }
            if(globalTimer%2==0)
                table.updateTable();
            table.matchNaive();

            if(!table.matchValid.empty()) {
                clearTimer = clearTimerMax;
            }
        } else if(clearTimer == 0) {
            for(int i=0; i<H; i++) {
                if(table.findMatch(table.getTableMatch(0, i)))
                    table.floodFillTable(0, i, Table::emptyEntry);
            }
        }

        table.resetAutoTexture();

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(raylib::Color(0xFFF0F5FF));

            raylib::DrawText(std::to_string(clearTimer), 150, 24, 32, BLACK);

            DrawSpriteAtlas(atlas, mainUI[0], TblULX, TblULY, 6, 6, 0, WHITE);

            DrawTexturePro(table.GetTexture(), (Rectangle){ 0, 0, W, -H},
                (Rectangle){ TblX, TblY, W*BlkW, H*BlkW },
                (Vector2){ (float)W*BlkW/2, (float)H*BlkW/2,}, 0.0f, WHITE);

            for(int i=0; i<H; i++) for(int j=0; j<W; j++) {
                if(table.findMatch(table.getTableMatch(j, i))) {
                    raylib::Vector2 UL(TblULX + BlkW*j, TblULY + BlkW*i);
                    raylib::Vector2 UR = UL.Add({BlkW, 0}), BL = UL.Add({0, BlkW}), BR = UL.Add({BlkW, BlkW});

                    int match = table.getTableMatch(j, i);

                    float timer1 = (clearTimer % 30) / 30.0f;
                    float timer2 = ((clearTimer + 15) % 30) / 30.0f;

                    auto movingline = [](raylib::Vector2 v1, raylib::Vector2 v2, float timer) {
                        float v = step(timer, .0f, 2);
                        auto v3 = v1.MoveTowards(v2, std::max(BlkW*(v-1), 0.0f) - 2.0f);
                        auto v4 = v1.MoveTowards(v2, std::min(BlkW*v, 8.0f) + 2.0f);
                        DrawLineEx(v3, v4, 4, BLACK);
                    };

                    if(table.isSafe(j-1, i) && table.getTableMatch(j-1, i) != match)
                        movingline(UL, BL, ((i+j)%2)?timer1:timer2);
                    if(table.isSafe(j+1, i) && table.getTableMatch(j+1, i) != match)
                        movingline(BR, UR, ((i+j)%2)?timer1:timer2);
                    if(table.isSafe(j, i-1) && table.getTableMatch(j, i-1) != match)
                        movingline(UR, UL, ((i+j+1)%2)?timer1:timer2);
                    if(table.isSafe(j, i+1) && table.getTableMatch(j, i+1) != match)
                        movingline(BL, BR, ((i+j+1)%2)?timer1:timer2);
                }
            }

            for(int i=0; i<4; i++) for(int j=0; j<4; j++) {
                if(getMino(curBlock, i, j)) {
                    DrawRectangle(TblULX + BlkW*(curBlock.x + 9*i), TblULY + BlkW*(curBlock.y + 9*j), BlkW*9, BlkW*9, colLighter(cmap[curBlock.col], -5));
                    DrawRectangle(TblULX + BlkW*(curBlock.x + 9*i +1), TblULY + BlkW*(curBlock.y + 9*j +1), BlkW*7, BlkW*7, colLighter(cmap[curBlock.col], 25));
                    DrawRectangle(TblULX + BlkW*(curBlock.x + 9*i +2), TblULY + BlkW*(curBlock.y + 9*j +2), BlkW*5, BlkW*5, colLighter(cmap[curBlock.col], -5));
                }
            }

            int tBlkW = 3;
            for(int n=0; n<4; n++) {
                DrawSpriteAtlas(atlas, mino_24[blockQueue[n+1].type], TblULX + nextOffset[n].x, TblULY + nextOffset[n].y, 3, 3, 0, cmap[blockQueue[n+1].col]);
            }

            if(holdBlock.type != -1) {
                DrawSpriteAtlas(atlas, mino_16[holdBlock.type], TblULX + holdOffset.x, TblULY + holdOffset.y, 6, 6, globalTimer, cmap[holdBlock.col]);
            }



        EndDrawing();
        //----------------------------------------------------------------------------------
    }

}
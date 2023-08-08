#ifndef GAMELOGIC_HPP
#define GAMELOGIC_HPP

#include<iostream>

#include "raylib.h"
#include "raylib-cpp.hpp"

#include "table.hpp"
#include "mino.hpp"
#include "helper.hpp"

static raylib::Color cmap[6] = {WHITE, RED, BLUE, GREEN, VIOLET, DARKBROWN};

class GameLogic {
private:
	BlockState blockQueue[5];
	BlockState& curBlock = blockQueue[0];
	BlockState holdBlock = {40, 0, 0, -1, 0};
	int W, H;
	Table table;

	int globalTimer = 0;
	bool clearing = false;
	int clearTimer = -1;
	const int clearTimerMax = 90;
public:
	GameLogic(int W, int H): W(W), H(H), table(W, H) {
	    popBlock();
	    popBlock();
	    popBlock();
	    popBlock();
	    popBlock();
	}

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

	void updateGame() {
        globalTimer++;
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
	}
};

#endif
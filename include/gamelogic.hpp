#ifndef GAMELOGIC_HPP
#define GAMELOGIC_HPP

#include <iostream>
#include <set>
#include <string>

#include "raylib.h"
#include "raylib-cpp.hpp"

#include "table.hpp"
#include "mino.hpp"
#include "helper.hpp"


class FlagObject {
private:
	std::set<std::string> flagSet;
protected:
	void setFlag(std::string name) {
		flagSet.insert(name);
	}
public:
	bool consumeFlag(std::string name) {
		if(flagSet.find(name) != flagSet.end()) {
			flagSet.erase(name);
			return true;
		}
		return false;
	}
	bool getFlag(std::string name) const {
		return flagSet.find(name) != flagSet.end();
	}
};


class GameLogic: public FlagObject {
private:
	int W, H;
	Table table;

	int globalTimer = 0;
	bool clearing = false;
	int clearTimer = -1;
	const int clearTimerMax = 90;
public:
	BlockState blockQueue[5];
	BlockState& curBlock = blockQueue[0];
	BlockState holdBlock = {40, 0, 0, -1, 0};
	
	GameLogic(int W, int H): W(W), H(H), table(W, H) {
	    popBlock();
	    popBlock();
	    popBlock();
	    popBlock();
	    popBlock();
	}

	void reset() {
		table.setAutoTexture();
	    table.clearTable();
		table.resetAutoTexture();
	    popBlock();
	    popBlock();
	    popBlock();
	    popBlock();
	    popBlock();
	}


	raylib::TextureUnmanaged getTexture() {
	    return table.getTexture();
	}

	bool isPositionMatched(int x, int y) {
		return table.findMatch(table.getTableMatch(x, y));
	}

	void popBlock() {
	    for(int i=0; i<4; i++)
	        blockQueue[i] = blockQueue[i+1];
	    blockQueue[4] = {40, 0, 0, GetRandomValue(0, 6), GetRandomValue(1, 5)};
	}

	bool colCheck(const BlockState& blk) {
	    for(int i=0; i<4; i++) for(int j=0; j<4; j++) {
	        if(!blk.get(i, j)) continue;
	        for(int a=0; a<9; a++) for(int b=0; b<9; b++) {
	            int tmpx = blk.x + i*9 + a, tmpy = blk.y + j*9 + b;
	            if(!table.isSafe(tmpx, tmpy) || table.getTableColor(tmpx, tmpy)!=0) return true;
	        }
	    }
	    return false;
	}
	bool colCheckWall(const BlockState& blk) {
	    for(int i=0; i<4; i++) for(int j=0; j<4; j++) {
	        if(!blk.get(i, j)) continue;
	        for(int a=0; a<9; a++) for(int b=0; b<9; b++) {
	            int tmpx = blk.x + i*9 + a, tmpy = blk.y + j*9 + b;
	            if(!table.isSafe(tmpx, tmpy)) return true;
	        }
	    }
	    return false;
	}

	void putBlock(const BlockState& blk) {
	    for(int i=0; i<4; i++) for(int j=0; j<4; j++) {
	        if(!blk.get(i, j)) continue;
	        for(int a=0; a<9; a++) for(int b=0; b<9; b++) {
	            int tmpx = blk.x + i*9 + a, tmpy = blk.y + j*9 + b;
	            table.setTableEntry(tmpx, tmpy, {blk.col, 0, brickImages[blk.col].GetColor(a, b)});
	        }
	    }
	}

	// Make sure that the block is not colliding with anything
	bool resolveCollision(BlockState& blk) {
		// First resolve collision with walls
	    while(colCheckWall(blk)) {
	        for(int i=0; i<4; i++) for(int j=0; j<4; j++) {
	            if(!blk.get(i, j)) continue;
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
		// Then resolve collision with other blocks
	    while(colCheck(blk)) {
	        blk.y--;
	    }

	    return false;
	}

	void update() {
		if(getFlag("gameover")) return;

        globalTimer++;
        clearTimer = std::max(clearTimer-1, -1);
        
		table.setAutoTexture();
        if(clearTimer == -1) {
            if(IsKeyPressed(KEY_F)) {
                if(holdBlock.type != -1) {
                    std::swap(curBlock, holdBlock);
                    curBlock.x = holdBlock.x;
                    curBlock.y = holdBlock.y;
                    resolveCollision(curBlock);
                } else {
                    holdBlock = curBlock;
                    popBlock();
                }
            }

            if(IsKeyDown(KEY_RIGHT)) {
                BlockState tmp = curBlock;
                tmp.x += 1;
                if(!colCheck(tmp)) {
                    curBlock = tmp;
                }
            }
            if(IsKeyDown(KEY_LEFT)) {
                BlockState tmp = curBlock;
                tmp.x -= 1;
                if(!colCheck(tmp)) {
                    curBlock = tmp;
                }
            }
            if(IsKeyPressed(KEY_UP)) {
                curBlock.rot = (curBlock.rot+1)%4;
                resolveCollision(curBlock);
            }
            int yspd = IsKeyPressed(KEY_SPACE) ? 9999 : (IsKeyDown(KEY_DOWN) ? 3 : 1);
            for(int i=0; i<yspd; i++) {
                BlockState tmp = curBlock;
                tmp.y += 1;
                if(!colCheck(tmp)) {
                    curBlock = tmp;
                } else {
                    putBlock(curBlock);
                    popBlock();
                    if(IsKeyPressed(KEY_SPACE)) {
                    	setFlag("POWERSHOCK");
                    } else if(IsKeyDown(KEY_DOWN)) {
                    	setFlag("SHOCK");
                    } else {
                    	setFlag("SOFTSHOCK");
                    }
					if(colCheck(curBlock)) {
						setFlag("GAMEOVER");
					}
                    break;
                }
            }
            if(globalTimer%2==0)
                table.updateTable();
            table.matchNaive();

            if(!table.matchValid.empty()) {
                clearTimer = clearTimerMax;
				setFlag("MATCH");
            }
        } else if(clearTimer == 0) {
            for(int i=0; i<H; i++) {
                if(table.findMatch(table.getTableMatch(0, i)))
                    table.floodFillTable(0, i, Table::emptyEntry);
            }
			consumeFlag("MATCH");
        }

        table.resetAutoTexture();
	}
};

#endif
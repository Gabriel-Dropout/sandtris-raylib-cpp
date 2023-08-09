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

// FLAG interface for communicating with the external environment
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
public:
	const int clearTimerMax = 60;
	const int comboTimerMax = 60*10;
private:
	int W, H;
	Table table;

	int globalTimer = 0;
	int clearTimer = -1;
	int comboTimer = 0;

	int score = 0;
	int combo = 0;

public:
	BlockState blockQueue[5];
	BlockState& curBlock = blockQueue[0];
	BlockState holdBlock = {40, 0, 0, -1, 0};
	float realx = 40;
	float hspd = 0;
	float acc = 0.2;
	float maxspd = 1.8;
	
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

		score = 0;
	}


	raylib::TextureUnmanaged getTexture() {
	    return table.getTexture();
	}

	int getScore() const {
		return score;
	}
	int getCombo() const {
		return combo;
	}
	int getComboTimer() const {
		return comboTimer;
	}

	bool isPositionMatched(int x, int y) {
		return table.findMatch(table.getTableMatch(x, y));
	}

	void popBlock() {
	    for(int i=0; i<4; i++)
	        blockQueue[i] = blockQueue[i+1];
	    blockQueue[4] = {40, 0, 0, GetRandomValue(0, 6), GetRandomValue(1, 5)};
	    realx = curBlock.x;
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
        comboTimer = std::max(comboTimer-1, 0);
		if(comboTimer == 0) {
			combo = 0;
		}
        
		table.setAutoTexture();
        if(clearTimer == -1) {
        	// hold
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

            // move: internally decimal point are used, but the block is always snapped to the grid
            if(IsKeyDown(KEY_RIGHT)) {
            	hspd = std::min(hspd+acc, maxspd);
            }
            if(IsKeyDown(KEY_LEFT)) {
            	hspd = std::max(hspd-acc, -maxspd);
            }
            if(!IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)) {
				hspd = hspd>0 ? std::max(hspd-acc*0.5f, 0.0f) : std::min(hspd+acc*0.5f, 0.0f);
			}
			realx += hspd;
			int xdiff = (int)realx - curBlock.x;
			while(xdiff != 0) {
				BlockState tmp = curBlock;
                tmp.x += xdiff>0 ? 1 : -1;
                xdiff += xdiff>0 ? -1 : 1;

                if(!colCheck(tmp)) {
                    curBlock = tmp;
                } else {
                	realx = curBlock.x;
                	hspd = 0;
                	break;
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
				setFlag("MATCH_ONCE");
				combo++;
				comboTimer = comboTimerMax;
            }
        } else if(clearTimer == 0) {
            score += table.clearMatchedEntry()/10;
			consumeFlag("MATCH");
        }

        table.resetAutoTexture();
	}
};

#endif
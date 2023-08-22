#ifndef MINO_HPP
#define MINO_HPP

#include "Functions.hpp"
#include "Image.hpp"
#include "raylib.h"
#include "raylib-cpp.hpp"


struct BlockStateBase {
    int x, y, rot, type, col[4];
};

// IJLOSTZ
inline constexpr unsigned long long minoBitmasks[] = {
    0x44440f00222200f0,
    0x0226047003220071,
    0x0622017002230074,
    0x0033003300330033,
    0x0462036002310036,
    0x0262027002320072,
    0x0264063001320063,
};
inline constexpr unsigned long long minoCoords[] = {
    0xea6289ab159d7654,
    0x9512456a15986540,
    0x951a456815906542,
    0x4051014515045410,
    0x51a6568959045421,
    0x9516456915946541,
    0x9562459a15486510,
};

extern raylib::Image brickImages[6];

inline bool getMino(const BlockStateBase& blk, int x, int y) {
    return ((minoBitmasks[blk.type] & (1ULL<<(16*blk.rot+4*y+x))) != 0);
}
inline Vector2 getMino(const BlockStateBase& blk, int n) {
    int _x = (minoCoords[blk.type]>>(blk.rot*16 + n*4)) & 3;
    int _y = (minoCoords[blk.type]>>(blk.rot*16 + n*4 + 2)) & 3;
    return {(float)_x, (float)_y};
}

// make mino as a class
class BlockState : public BlockStateBase {
public:
    BlockState(): BlockStateBase{} {}
    BlockState(int x, int y, int rot, int type, int col): BlockStateBase{x, y, rot, type, {col, col, col, col}} {}
    BlockState(int x, int y, int rot, int type, int col[4]): BlockStateBase{x, y, rot, type, {col[0], col[1], col[2], col[3]}} {}
    BlockState(const BlockStateBase& blk): BlockStateBase{blk} {}

    bool get(int x, int y) const {
        return getMino(*this, x, y);
    }
    Vector2 get(int n) const {
        return getMino(*this, n);
    }
};

#endif


#ifndef MINO_HPP
#define MINO_HPP

#include "raylib.h"
#include "raylib-cpp.hpp"


struct BlockStateBase {
    int x, y, rot, type, col;
};

// IJLOSTZ
inline constexpr unsigned long long minos[] = {
    0x44440f00222200f0,
    0x0226047003220071,
    0x0622017002230074,
    0x0033003300330033,
    0x0462036002310036,
    0x0262027002320072,
    0x0264063001320063,
};

inline bool getMino(const BlockStateBase& blk, int x, int y) {
    return ((minos[blk.type] & (1ULL<<(16*blk.rot+4*y+x))) != 0);
}

// make mino as a class
class BlockState : public BlockStateBase {
public:
    BlockState(): BlockStateBase{} {}
    BlockState(int x, int y, int rot, int type, int col): BlockStateBase{x, y, rot, type, col} {}
    BlockState(const BlockStateBase& blk): BlockStateBase{blk} {}

    bool get(int x, int y) const {
        return getMino(*this, x, y);
    }
};

#endif
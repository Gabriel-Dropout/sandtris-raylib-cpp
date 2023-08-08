#ifndef MINO_HPP
#define MINO_HPP

#include "raylib.h"
#include "raylib-cpp.hpp"


struct BlockState {
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

inline bool getMino(const BlockState& blk, int x, int y) {
    return ((minos[blk.type] & (1ULL<<(16*blk.rot+4*y+x))) != 0);
}

#endif
#ifndef ZOBRIST_HASH_H
#define ZOBRIST_HASH_H

#include <random>
#include <cstdint>
#include "../../common/game_constants.h"

namespace zobrist_hash
{
    extern std::mt19937 mt_init_hash;

    extern uint64_t points[GameConstants::Board::H][GameConstants::Board::W][10]; // 숫자 0-9 이용
    extern uint64_t character[GameConstants::Board::H][GameConstants::Board::W];
    
    void init();
}

#endif // ZOBRIST_HASH_H
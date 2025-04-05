#include "zobrist_hash.h"

namespace zobrist_hash
{
    std::mt19937 mt_init_hash(0);
    
    // 벽 위치는 고정이므로 해시 관리하지 않음
    uint64_t points[GameConstants::Board::H][GameConstants::Board::W][10] = {}; // 숫자 0-9 이용
    uint64_t character[GameConstants::Board::H][GameConstants::Board::W] = {};
    
    void init()
    {
        for (int y = 0; y < GameConstants::Board::H; y++)
            for (int x = 0; x < GameConstants::Board::W; x++)
            {
                for (int p = 0; p < 10; p++)
                {
                    points[y][x][p] = mt_init_hash();
                }
                character[y][x] = mt_init_hash();
            }
    }
}
#ifndef WALLMAZE_BITSET_H
#define WALLMAZE_BITSET_H

#include <bitset>
#include "../../common/coord.h"
#include "../../common/game_constants.h"

namespace single_bitset
{
    std::bitset<GameConstants::Board::H *GameConstants::Board::W> initLeftMask();
    std::bitset<GameConstants::Board::H *GameConstants::Board::W> initRightMask();

    // 2D 배열을 단일 비트셋으로 표현하는 클래스
    class Mat {
    private:
        std::bitset<GameConstants::Board::H *GameConstants::Board::W> bits_;

        Mat up_mat() const;
        Mat down_mat() const;
        Mat left_mat() const;
        Mat right_mat() const;
    
    public:
        Mat() = default;
        Mat(const std::bitset<GameConstants::Board::H *GameConstants::Board::W> &mat);

        bool get(int y, int x) const;
        void set(int y, int x);
        void del(int y, int x);
        void expand();
        void andeq_not(const Mat& mat);
        bool is_equal(const Mat& mat) const;
        bool is_any_equal(const Mat& mat) const;
    };    
}

#endif // WALLMAZE_BITSET_H

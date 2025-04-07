#include "wallmaze_bitset.h"
#include "../../common/game_constants.h"

namespace single_bitset
{
    std::bitset<GameConstants::Board::H * GameConstants::Board::W> left_mask = initLeftMask();
    std::bitset<GameConstants::Board::H * GameConstants::Board::W> right_mask = initRightMask();
    
    std::bitset<GameConstants::Board::H * GameConstants::Board::W> initLeftMask()
    {
        std::bitset<GameConstants::Board::H * GameConstants::Board::W> mask;
        for (int y = 0; y < GameConstants::Board::H; ++y)
        {
            mask |= (std::bitset<GameConstants::Board::H * GameConstants::Board::W>(1) 
                << (y * GameConstants::Board::W));
        }
        mask = ~mask;  // 왼쪽 경계를 제외한 모든 위치를 1로 설정
        return mask;
    }
    
    std::bitset<GameConstants::Board::H * GameConstants::Board::W> initRightMask()
    {
        std::bitset<GameConstants::Board::H * GameConstants::Board::W> mask;
        for (int y = 0; y < GameConstants::Board::H; ++y)
        {
            mask |= (std::bitset<GameConstants::Board::H * GameConstants::Board::W>(1) 
                << (y * GameConstants::Board::W + GameConstants::Board::W - 1));
        }
        mask = ~mask;  // 오른쪽 경계를 제외한 모든 위치를 1로 설정
        return mask;
    }
    
    Mat::Mat(const std::bitset<GameConstants::Board::H * GameConstants::Board::W> &mat) 
        : bits_(mat) {}
    
    bool Mat::get(int y, int x) const
    {
        return bits_[y * GameConstants::Board::W + x];
    }
    
    void Mat::set(int y, int x)
    {
        bits_.set(y * GameConstants::Board::W + x);
    }
    
    void Mat::del(int y, int x)
    {
        bits_.reset(y * GameConstants::Board::W + x);
    }
    
    Mat Mat::up_mat() const
    {
        Mat ret_mat = *this;
        ret_mat.bits_ >>= GameConstants::Board::W;
        return ret_mat;
    }
    
    Mat Mat::down_mat() const
    {
        Mat ret_mat = *this;
        ret_mat.bits_ <<= GameConstants::Board::W;
        return ret_mat;
    }
    
    Mat Mat::left_mat() const
    {
        Mat ret_mat = *this;
        ret_mat.bits_ |= (ret_mat.bits_ & left_mask) >> 1;
        return ret_mat;
    }
    
    Mat Mat::right_mat() const
    {
        Mat ret_mat = *this;
        ret_mat.bits_ |= (ret_mat.bits_ & right_mask) << 1;
        return ret_mat;
    }
    
    void Mat::expand()
    {
        Mat up = up_mat();
        Mat down = down_mat();
        Mat left = left_mat();
        Mat right = right_mat();
        
        this->bits_ |= up.bits_;
        this->bits_ |= down.bits_;
        this->bits_ |= left.bits_;
        this->bits_ |= right.bits_;
    }
    
    // NAND 연산
    void Mat::andeq_not(const Mat &mat)
    {
        this->bits_ &= ~mat.bits_;
    }
    
    bool Mat::is_equal(const Mat &mat) const
    {
        return this->bits_ == mat.bits_;
    }
    
    bool Mat::is_any_equal(const Mat &mat) const
    {
        return (this->bits_ & mat.bits_).any();
    }
}
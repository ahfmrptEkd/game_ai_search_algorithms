#ifndef CONNECT_FOUR_BITBOARD_H
#define CONNECT_FOUR_BITBOARD_H

#include "connect_four_state.h"
#include "../../common/game_util.h"
#include <cstdint>
#include <vector>
#include <string>
#include <sstream>

class ConnectFourBitBoardState {
private:
    uint64_t my_board_ = 0ULL;
    uint64_t all_board_ = 0ULL;
    bool is_first_ = true;
    WinningStatus winning_status_ = WinningStatus::NONE;

    bool isWinner(const uint64_t board);

public:
    ConnectFourBitBoardState();
    
    // ConnectFourState로부터 비트보드 상태 생성 (변환 생성자)
    ConnectFourBitBoardState(const ConnectFourState& state);
    
    // 게임 상태 관련 메서드
    bool isDone() const;
    void advance(const int action);
    std::vector<int> legalActions() const;
    WinningStatus getWinningStatus() const;
    std::string toString() const;
    
    double getFirstPlayerScoreForWinRate() const;
    
    bool isValidMove(int col) const;
};

#endif // CONNECT_FOUR_BITBOARD_H
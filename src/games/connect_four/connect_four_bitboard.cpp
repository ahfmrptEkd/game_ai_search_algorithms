#include "connect_four_bitboard.h"

ConnectFourBitBoardState::ConnectFourBitBoardState() 
    : my_board_(0ULL), all_board_(0ULL), is_first_(true), winning_status_(WinningStatus::NONE)
{
}

ConnectFourBitBoardState::ConnectFourBitBoardState(const ConnectFourState& state) 
    : is_first_(state.isFirst())
{
    my_board_ = 0ULL;
    all_board_ = 0ULL;
    
    // 기존 배열 기반 보드를 비트보드로 변환
    for (int y = 0; y < GameConstants::ConnectFour::H; y++) {
        for (int x = 0; x < GameConstants::ConnectFour::W; x++) {
            int index = x * (GameConstants::ConnectFour::H + 1) + y;
            
            if (state.getMyBoard()[y * GameConstants::ConnectFour::W + x] == 1) {
                this->my_board_ |= 1ULL << index;
            }
            
            if (state.getMyBoard()[y * GameConstants::ConnectFour::W + x] == 1 || 
                state.getEnemyBoard()[y * GameConstants::ConnectFour::W + x] == 1) {
                this->all_board_ |= 1ULL << index;
            }
        }
    }
}

bool ConnectFourBitBoardState::isWinner(const uint64_t board)
{
    // 가로 방향으로 연속인가 판정한다.
    uint64_t tmp_board = board & (board >> 7);
    if ((tmp_board & (tmp_board >> 14)) != 0) {
        return true;
    }
    
    // "\"방향으로 연속인가 판정한다.
    tmp_board = board & (board >> 6);
    if ((tmp_board & (tmp_board >> 12)) != 0) {
        return true;
    }
    
    // "／"방향으로 연속인가 판정한다.
    tmp_board = board & (board >> 8);
    if ((tmp_board & (tmp_board >> 16)) != 0) {
        return true;
    }
    
    // 세로 방향으로 연속인가 판정한다.
    tmp_board = board & (board >> 1);
    if ((tmp_board & (tmp_board >> 2)) != 0) {
        return true;
    }

    return false;
}

bool ConnectFourBitBoardState::isDone() const
{
    return winning_status_ != WinningStatus::NONE;
}

void ConnectFourBitBoardState::advance(const int action)
{
    // 상대방의 시점으로 바뀐다
    this->my_board_ ^= this->all_board_;
    is_first_ = !is_first_;
    
    // 비트보드의 열에 돌을 추가
    uint64_t new_all_board = this->all_board_ | (this->all_board_ + (1ULL << (action * 7)));
    this->all_board_ = new_all_board;
    
    // 가득 찬 보드를 나타내는 비트마스크
    uint64_t filled = 0b0111111011111101111110111111011111101111110111111ULL;

    // 승패 확인
    if (isWinner(this->my_board_ ^ this->all_board_)) {
        this->winning_status_ = WinningStatus::LOSE;
    }
    else if (this->all_board_ == filled) {
        this->winning_status_ = WinningStatus::DRAW;
    }
}

std::vector<int> ConnectFourBitBoardState::legalActions() const
{
    std::vector<int> actions;
    
    // 각 열의 최상단 비트를 확인하는 마스크
    uint64_t possible = this->all_board_ + 0b0000001000000100000010000001000000100000010000001ULL;
    uint64_t filter = 0b0111111;
    
    for (int x = 0; x < GameConstants::ConnectFour::W; x++) {
        if ((filter & possible) != 0) {
            actions.emplace_back(x);
        }
        filter <<= 7;
    }
    
    return actions;
}

WinningStatus ConnectFourBitBoardState::getWinningStatus() const
{
    return this->winning_status_;
}

std::string ConnectFourBitBoardState::toString() const
{
    std::stringstream ss;
    
    ss << "Current turn: " << (this->is_first_ ? "X" : "O") << "\n\n";
    
    // 열 번호 표시
    ss << "    1   2   3   4   5   6   7\n";
    ss << "  +---+---+---+---+---+---+---+\n";

    for (int y = GameConstants::ConnectFour::H - 1; y >= 0; y--) {
        ss << "  |";
        for (int x = 0; x < GameConstants::ConnectFour::W; x++) {
            int index = x * (GameConstants::ConnectFour::H + 1) + y;
            char c = ' ';
            
            if (((my_board_ >> index) & 1ULL) != 0) {   
                c = (is_first_ ? 'X' : 'O');
            }
            else if ((((all_board_ ^ my_board_) >> index) & 1ULL) != 0) {
                c = (is_first_ ? 'O' : 'X');
            }
            
            ss << " " << c << " |";
        }
        ss << "\n";
        ss << "  +---+---+---+---+---+---+---+\n";
    }

    return ss.str();
}

double ConnectFourBitBoardState::getFirstPlayerScoreForWinRate() const
{
    switch (this->getWinningStatus()) {
    case (WinningStatus::WIN):
        if (this->is_first_) {
            return 1.;
        }
        else {
            return 0.;
        }
    case (WinningStatus::LOSE):
        if (this->is_first_) {
            return 0.;
        }
        else {
            return 1.;
        }
    default:
        return 0.5;
    }
}

bool ConnectFourBitBoardState::isValidMove(int col) const
{
    // 열이 유효한 범위에 있는지 확인
    if (col < 0 || col >= GameConstants::ConnectFour::W) {
        return false;
    }
    
    // 해당 열이 가득 찼는지 확인
    uint64_t top_mask = 1ULL << ((col + 1) * (GameConstants::ConnectFour::H + 1) - 2);
    return (all_board_ & top_mask) == 0;
}
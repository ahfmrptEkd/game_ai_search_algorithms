#include "connect_four_state.h"
#include "../../common/game_util.h"
#include <deque>
#include <sstream>
#include <utility>

ConnectFourState::ConnectFourState() {}

ConnectFourState::ConnectFourState(const int seed)
{
    is_first_ = true;
    winning_status_ = WinningStatus::NONE;

    for (int y = 0; y < GameConstants::ConnectFour::H; ++y) {
        for (int x = 0; x < GameConstants::ConnectFour::W; ++x) {
            my_board_[y][x] = 0;
            enemy_board_[y][x] = 0;
        }
    }
}

bool ConnectFourState::isDone() const 
{
    return winning_status_ != WinningStatus::NONE;
}

void ConnectFourState::progress(const int action)
{
    std::pair<int, int> coordinate;
    
    for (int y = 0; y < GameConstants::ConnectFour::H; ++y) {
        if (this->my_board_[y][action] == 0 && this->enemy_board_[y][action] == 0) {
            this->my_board_[y][action] = 1;
            coordinate = std::pair<int, int>(y, action);
            break;
        }
    }

    {
        // 가로 방향으로 연속인지 판단
        auto que = std::deque<std::pair<int, int>>();
        que.emplace_back(coordinate);
        std::vector<std::vector<bool>> check(GameConstants::ConnectFour::H, std::vector<bool>(GameConstants::ConnectFour::W, false));

        int count = 0;

        while (!que.empty()) {
            const auto& tmp_cod = que.front();
            que.pop_front();
            ++count;

            if (count >= 4) {
                this->winning_status_ = WinningStatus::LOSE; // 내 돌이 4개 연속, 상대방의 패배
                break;
            }

            check[tmp_cod.first][tmp_cod.second] = true;

            for (int action = 0; action < 2; action++) {
                int ty = tmp_cod.first;
                int tx = tmp_cod.second + dx[action];

                if (ty >= 0 && ty < GameConstants::ConnectFour::H && tx >= 0 && tx < GameConstants::ConnectFour::W && my_board_[ty][tx] == 1 && !check[ty][tx]) {
                    que.emplace_back(ty, tx);
                }
            }
        }
    }

    if (!isDone())
    { // "／" 방향 체크
        auto que = std::deque<std::pair<int, int>>();
        que.emplace_back(coordinate);
        std::vector<std::vector<bool>> check(GameConstants::ConnectFour::H, 
                                           std::vector<bool>(GameConstants::ConnectFour::W, false));
        int count = 0;
        while (!que.empty())
        {
            const auto &tmp_cod = que.front();
            que.pop_front();
            ++count;
            if (count >= 4)
            {
                this->winning_status_ = WinningStatus::LOSE;
                break;
            }
            check[tmp_cod.first][tmp_cod.second] = true;

            for (int action = 0; action < 2; action++)
            {
                int ty = tmp_cod.first + dy_right_up[action];
                int tx = tmp_cod.second + dx[action];

                if (ty >= 0 && ty < GameConstants::ConnectFour::H && 
                    tx >= 0 && tx < GameConstants::ConnectFour::W && 
                    my_board_[ty][tx] == 1 && !check[ty][tx])
                {
                    que.emplace_back(ty, tx);
                }
            }
        }
    }

    if (!isDone())
    { // "\"방향 체크
        auto que = std::deque<std::pair<int, int>>();
        que.emplace_back(coordinate);
        std::vector<std::vector<bool>> check(GameConstants::ConnectFour::H, 
                                           std::vector<bool>(GameConstants::ConnectFour::W, false));
        int count = 0;
        while (!que.empty())
        {
            const auto &tmp_cod = que.front();
            que.pop_front();
            ++count;
            if (count >= 4)
            {
                this->winning_status_ = WinningStatus::LOSE;
                break;
            }
            check[tmp_cod.first][tmp_cod.second] = true;

            for (int action = 0; action < 2; action++)
            {
                int ty = tmp_cod.first + dy_left_up[action];
                int tx = tmp_cod.second + dx[action];

                if (ty >= 0 && ty < GameConstants::ConnectFour::H && 
                    tx >= 0 && tx < GameConstants::ConnectFour::W && 
                    my_board_[ty][tx] == 1 && !check[ty][tx])
                {
                    que.emplace_back(ty, tx);
                }
            }
        }
    }

    if (!isDone()) 
    {
        // 세로 방향 체크
        int ty = coordinate.first;
        int tx = coordinate.second;
        bool is_win = true;

        for (int i = 0; i < 4; i++) {
            bool is_mine = (ty >= 0 && ty < GameConstants::ConnectFour::H && tx >= 0 && tx < GameConstants::ConnectFour::W && my_board_[ty][tx] == 1);

            if (!is_mine) {
                is_win = false;
                break;
            }

            --ty;
        }

        if (is_win) {
            this->winning_status_ = WinningStatus::LOSE;
        }
    }

    std::swap(my_board_, enemy_board_);
    is_first_ = !is_first_;

    // 가능한 행동이 없는 경우 무승부
    if (this->winning_status_ == WinningStatus::NONE && legalActions().empty()) {
        this->winning_status_ = WinningStatus::DRAW;
    }
}

std::vector<int> ConnectFourState::legalActions() const
{
    std::vector<int> actions;

    for (int x = 0; x < GameConstants::ConnectFour::W; x++) {
        for (int y = GameConstants::ConnectFour::H - 1; y >= 0; y--) {
            if (my_board_[y][x] == 0 && enemy_board_[y][x] == 0) {
                actions.emplace_back(x);
                break;
            }
        }
    }
    return actions;
}

std::string ConnectFourState::toString() const
{
    std::stringstream ss;
    
    ss << "Current turn: " << (this->is_first_ ? "X" : "O") << "\n\n";
    
    // 열 번호 표시
    ss << "    1   2   3   4   5   6   7\n";
    ss << "  +---+---+---+---+---+---+---+\n";
    
    // 보드 내용 표시 (테두리 포함)
    for (int y = GameConstants::ConnectFour::H - 1; y >= 0; --y) {
        ss << "  |";
        for (int x = 0; x < GameConstants::ConnectFour::W; ++x) {
            char c = ' ';
            
            if (my_board_[y][x] == 1) {
                c = (is_first_ ? 'X' : 'O');
            }
            else if (enemy_board_[y][x] == 1) {
                c = (is_first_ ? 'O' : 'X');
            }
            
            // 가운데에 한 칸 공백을 둔 뒤 문자 출력
            ss << " " << c << " |";
        }
        ss << "\n";
        ss << "  +---+---+---+---+---+---+---+\n";
    }
    
    return ss.str();
}

ScoreType ConnectFourState::evaluateScore()
{
    // 간단한 휴리스틱 평가 함수
    // 승리/패배/무승부 상태는 큰 값으로, 그 외에는 중앙 칸에 가중치를 둠
    if (isDone()) {
        switch (winning_status_) {
            case WinningStatus::WIN:
                return GameConstants::INF;
            case WinningStatus::LOSE:
                return -GameConstants::INF;
            case WinningStatus::DRAW:
                return 0;
            default:
                break;
        }
    }
    
    // 중앙 칸에 가중치를 두어 평가
    int score = 0;
    for (int y = 0; y < GameConstants::ConnectFour::H; y++) {
        for (int x = 0; x < GameConstants::ConnectFour::W; x++) {
            
            // 중앙에 가까울수록 가중치 증가 (중앙 열이 가장 가치 있음)
            int weight = GameConstants::ConnectFour::W - abs(x - GameConstants::ConnectFour::W/2);
            
            if (my_board_[y][x] == 1) {
                score += weight;
            } else if (enemy_board_[y][x] == 1) {
                score -= weight;
            }
        }
    }
    
    evaluated_score_ = score;
    return evaluated_score_;
}

WinningStatus ConnectFourState::getWinningStatus() const
{
    return winning_status_;
}

double ConnectFourState::getFirstPlayerScoreForWinRate() const
{
    switch (this->getWinningStatus())
    {
    case (WinningStatus::WIN):
        if (this->is_first_)
        {
            return 1.;
        }
        else
        {
            return 0.;
        }
    
    case (WinningStatus::LOSE):
        if (this->is_first_)
        {
            return 0.;
        }
        else
        {
            return 1.;
        }
    default:
        return 0.5;
    }
}
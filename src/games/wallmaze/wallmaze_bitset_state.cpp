#include "wallmaze_bitset_state.h"
#include <sstream>
#include <unordered_set>

WallMazeBitSetState::WallMazeBitSetState(const WallMazeState &state) 
    : turn_(state.getTurn()), 
      character_(state.getCharacterPosition()), 
      game_score_(state.getScore()) 
{
    // 벽과 점수 정보를 비트셋으로 변환
    for (int y = 0; y < GameConstants::Board::H; y++) {
        for (int x = 0; x < GameConstants::Board::W; x++) {
            if (state.hasWall(y, x)) {
                this->walls_.set(y, x);
            }
            if (state.getPoint(y, x) > 0) {
                this->points_[y][x] = state.getPoint(y, x);
                this->whole_point_mat_.set(y, x);
            }
        }
    }
    
    init_hash();
}

void WallMazeBitSetState::init_hash()
{
    hash_ = 0;
    hash_ ^= zobrist_hash::character[character_.y_][character_.x_];
    
    for (int y = 0; y < GameConstants::Board::H; y++) {
        for (int x = 0; x < GameConstants::Board::W; x++) {
            auto point = points_[y][x];
            if (point > 0) {
                hash_ ^= zobrist_hash::points[y][x][point];
            }
        }
    }
}

bool WallMazeBitSetState::isDone() const
{
    return this->turn_ == GameConstants::Board::END_TURN;
}

int WallMazeBitSetState::getDistanceToNearestPoint()
{
    auto now = Mat();
    now.set(this->character_.y_, this->character_.x_);
    
    for (int depth = 0;; ++depth) {
        if (now.is_any_equal(this->whole_point_mat_)) {
            return depth;
        }
        
        // 다음 확장 영역 계산
        auto next = now;
        next.expand();
        next.andeq_not(this->walls_); // 벽 위치 제외
        
        if (next.is_equal(now)) {
            break;
        }
        
        now = next;
    }
    
    return GameConstants::Board::H * GameConstants::Board::W;
}

void WallMazeBitSetState::evaluateScore()
{
    this->evaluated_score_ = this->game_score_ * GameConstants::Board::H * GameConstants::Board::W 
                            - getDistanceToNearestPoint();
}

void WallMazeBitSetState::advance(const int action)
{
    static constexpr const int dx[4] = {1, -1, 0, 0};
    static constexpr const int dy[4] = {0, 0, 1, -1};

    // 해시에서 현재 캐릭터 위치 정보 제거
    hash_ ^= zobrist_hash::character[character_.y_][character_.x_];
    
    // 캐릭터 이동
    this->character_.x_ += dx[action];
    this->character_.y_ += dy[action];
    
    // 해시에 새 캐릭터 위치 정보 추가
    hash_ ^= zobrist_hash::character[character_.y_][character_.x_];
    
    // 점수 획득 처리
    auto &point = this->points_[this->character_.y_][this->character_.x_];
    if (point > 0) {
        // 점수가 제거됨을 해시에 반영
        hash_ ^= zobrist_hash::points[character_.y_][character_.x_][point];
        
        // 점수 획득 및 맵에서 제거
        this->game_score_ += point;
        whole_point_mat_.del(this->character_.y_, this->character_.x_);
        point = 0;
    }
    
    this->turn_++;
}

std::vector<int> WallMazeBitSetState::legalActions() const
{
    static constexpr const int dx[4] = {1, -1, 0, 0};
    static constexpr const int dy[4] = {0, 0, 1, -1};
    
    std::vector<int> actions;
    
    for (int action = 0; action < 4; action++) {
        int ty = this->character_.y_ + dy[action];
        int tx = this->character_.x_ + dx[action];
        
        if (ty >= 0 && ty < GameConstants::Board::H && 
            tx >= 0 && tx < GameConstants::Board::W && 
            !this->walls_.get(ty, tx)) {
            actions.emplace_back(action);
        }
    }
    
    return actions;
}

std::string WallMazeBitSetState::toString() const
{
    std::stringstream ss;
    ss << "turn:\t" << this->turn_ << "\n";
    ss << "score:\t" << this->game_score_ << "\n";
    
    for (int h = 0; h < GameConstants::Board::H; h++) {
        for (int w = 0; w < GameConstants::Board::W; w++) {
            if (this->walls_.get(h, w)) {
                ss << '#';
            }
            else if (this->character_.y_ == h && this->character_.x_ == w) {
                ss << '@';
            }
            else if (this->points_[h][w] > 0) {
                ss << points_[h][w];
            }
            else {
                ss << '.';
            }
        }
        ss << '\n';
    }
    
    return ss.str();
}

bool WallMazeBitSetState::operator<(const WallMazeBitSetState& other) const
{
    return this->evaluated_score_ < other.evaluated_score_;
}

int beamSearchActionBitset(const WallMazeBitSetState &state, int beam_width, int beam_depth)
{
    std::priority_queue<WallMazeBitSetState> now_beam;
    WallMazeBitSetState best_state;
    
    now_beam.push(state);
    auto hash_check = std::unordered_set<uint64_t>();
    
    for (int t = 0; t < beam_depth; t++) {
        std::priority_queue<WallMazeBitSetState> next_beam;
        
        for (int i = 0; i < beam_width; i++) {
            if (now_beam.empty())
                break;
                
            WallMazeBitSetState now_state = now_beam.top();
            now_beam.pop();
            
            auto legal_actions = now_state.legalActions();
            for (const auto &action : legal_actions) {
                WallMazeBitSetState next_state = now_state;
                next_state.advance(action);
                
                if (t >= 1 && hash_check.count(next_state.hash_) > 0) {
                    continue;
                }
                
                hash_check.emplace(next_state.hash_);
                next_state.evaluateScore();
                
                if (t == 0)
                    next_state.first_action_ = action;
                    
                next_beam.push(next_state);
            }
        }
        
        now_beam = next_beam;
        
        if (now_beam.empty())
            break;
            
        best_state = now_beam.top();
        
        if (best_state.isDone()) {
            break;
        }
    }
    
    return best_state.first_action_;
}
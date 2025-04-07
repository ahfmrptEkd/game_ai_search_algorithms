#ifndef WALLMAZE_BITSET_STATE_H
#define WALLMAZE_BITSET_STATE_H

#include "wallmaze_bitset.h"
#include "wallmaze_state.h"
#include "zobrist_hash.h"
#include <unordered_set> 
#include <queue>         

using single_bitset::Mat;

class WallMazeBitSetState {
private:
    int points_[GameConstants::Board::H][GameConstants::Board::W] = {};
    Mat whole_point_mat_ = Mat();
    int turn_ = 0;
    Mat walls_ = Mat();
    Coord character_ = Coord();

    void init_hash();
    
public:
    int game_score_ = 0;
    GameConstants::ScoreType evaluated_score_ = 0;
    int first_action_ = -1;
    uint64_t hash_ = 0;
    
    WallMazeBitSetState() = default;
    explicit WallMazeBitSetState(const WallMazeState &state);
    
    void evaluateScore();
    bool isDone() const;
    void advance(const int action);
    std::vector<int> legalActions() const;
    std::string toString() const;
    
    bool operator<(const WallMazeBitSetState& other) const;
    int getDistanceToNearestPoint();
};

int beamSearchActionBitset(const WallMazeBitSetState &state, int beam_width, int beam_depth);

#endif // WALLMAZE_BITSET_STATE_H
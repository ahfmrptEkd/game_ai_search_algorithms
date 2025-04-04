#ifndef MONTECARLO_H
#define MONTECARLO_H

#include "../../../games/twomaze/twomaze_state.h"
#include "../../../common/game_util.h"
#include "random.h"
namespace monte_carlo {
    // 몬테카를로 플레이아웃 - 랜덤 플레이아웃으로 게임 종료시 승패 점수 반환
    double playout(TwoMazeState state);

    // 몬테카를로 행동 선택
    int monteCarloAction(const TwoMazeState& state, const int playout_number);
}

int monteCarloSearchAction(const TwoMazeState& state, const int playout_number);

inline void playGameMonteCarlo(const int seed, int playout_number = GameConstants::TwoMaze::PLAYOUT_NUMBER) {
    playTwoMazeGame([playout_number](const TwoMazeState& state) {
        return monteCarloSearchAction(state, playout_number);
    }, seed);
}

#endif // MONTECARLO_H
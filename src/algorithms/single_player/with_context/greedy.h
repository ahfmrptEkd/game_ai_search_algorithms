#ifndef GREEDY_ALGORITHM_H
#define GREEDY_ALGORITHM_H

#include "../../../games/maze/maze_state.h"
#include "../../../common/game_util.h"

// 탐욕법으로 행동을 결정한다.
int greedyAction(const MazeState& state);

// 시드를 지정해서 게임을 탐욕법 알고리즘으로 진행
inline void playGameGreedy(const int seed) {
    playGameWithStrategy<MazeState>(seed, greedyAction);
}

#endif // GREEDY_ALGORITHM_H

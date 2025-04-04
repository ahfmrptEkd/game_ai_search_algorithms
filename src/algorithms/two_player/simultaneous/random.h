#ifndef SIM_RANDOM_H
#define SIM_RANDOM_H

#include "../../../games/simmaze/simmaze_state.h"
#include <functional>

// 무작위 행동 알고리즘 - 특정 플레이어의 무작위 행동 선택
int simMazeRandomAction(const SimMazeState& state, const int player_id);

// 두 플레이어의 행동을 각각 받아 게임 진행
void playSimMazeGame(
    const std::function<int(const SimMazeState&, const int)>& action0_func,
    const std::function<int(const SimMazeState&, const int)>& action1_func,
    const int seed);

// 시드를 고정해서 게임을 무작위 알고리즘으로 진행
inline void playSimMazeGameRandom(const int seed) {
    playSimMazeGame(
        [](const SimMazeState& state, const int player_id) {
            return simMazeRandomAction(state, player_id);
        },
        [](const SimMazeState& state, const int player_id) {
            return simMazeRandomAction(state, player_id);
        },
        seed);
}

#endif // SIM_RANDOM_H
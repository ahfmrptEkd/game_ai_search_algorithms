#ifndef RANDOM_ALGORITHM_WITHOUT_CONTEXT_H
#define RANDOM_ALGORITHM_WITHOUT_CONTEXT_H

#include "../../../games/automaze/automaze_state.h"

// 캐릭터를 랜덤하게 배치하는 함수
AutoMazeState randomPlacement(const AutoMazeState &state);

// 게임 실행 함수
void playAutoMazeGame(const std::string& ai_name, AutoMazeState (*ai_func)(const AutoMazeState &), const int seed);

// 랜덤 AI 게임 플레이 함수
inline void playAutoMazeGameRandom(const int seed) {
    playAutoMazeGame("RandomPlacement", randomPlacement, seed);
}

#endif // RANDOM_ALGORITHM_WITHOUT_CONTEXT_H
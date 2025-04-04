#include "random.h"
#include "../../../common/game_util.h"
#include <iostream>
#include <random>

// 랜덤 배치 알고리즘 구현
AutoMazeState randomPlacement(const AutoMazeState& state)
{
    AutoMazeState now_state = state;
    for (int character_id = 0; character_id < GameConstants::AutoMaze::CHARACTER_N; character_id++)
    {
        int y = GameUtil::mt_for_action() % GameConstants::Board::H;
        int x = GameUtil::mt_for_action() % GameConstants::Board::W;

        now_state.setCharacter(character_id, y, x);
    }
    return now_state;
}

// 게임을 1회 플레이해서 게임 상황을 표시하는 함수
void playAutoMazeGame(const std::string &ai_name, AutoMazeState (*ai_func)(const AutoMazeState &), const int seed) 
{
    auto state = AutoMazeState(seed);
    state = ai_func(state);
    auto score = state.getScore(true);
}
#include "simulated_annealing.h"
#include "../../../common/game_util.h"
#include <random>
#include <cmath>
#include <iostream>

// 모의 담금질 배치 알고리즘 구현
AutoMazeState simulatedAnnealingPlacement(const AutoMazeState &state, int number, double start_temp, double end_temp)
{
    AutoMazeState now_state = state;
    for (int character_id = 0; character_id < GameConstants::AutoMaze::CHARACTER_N; character_id++)
    {
        int y = GameUtil::mt_for_action() % GameConstants::Board::H;
        int x = GameUtil::mt_for_action() % GameConstants::Board::W;
        now_state.setCharacter(character_id, y, x);
    }
    
    ScoreType best_score = now_state.getScore();
    ScoreType now_score = best_score;
    auto best_state = now_state;

    for (int i = 0; i < number; i++)
    {
        auto next_state = now_state;
        
        int character_id = GameUtil::mt_for_action() % GameConstants::AutoMaze::CHARACTER_N;
        int y = GameUtil::mt_for_action() % GameConstants::Board::H;
        int x = GameUtil::mt_for_action() % GameConstants::Board::W;
        next_state.setCharacter(character_id, y, x);
        
        auto next_score = next_state.getScore();
        
        // 온도 계산 (선형적으로 감소)
        double temp = start_temp + (end_temp - start_temp) * (static_cast<double>(i) / number);
        
        // 확률 계산 (점수가 더 낮더라도 일정 확률로 수락)
        double probability = exp((next_score - now_score) / temp);
        bool is_force_next = probability > (GameUtil::mt_for_action() % GameConstants::INF) / static_cast<double>(GameConstants::INF);
        
        // 더 좋은 해이거나 확률적으로 선택된 경우 현재 상태 업데이트
        if (next_score > now_score || is_force_next)
        {
            now_score = next_score;
            now_state = next_state;
        }

        if (next_score > best_score)
        {
            best_score = next_score;
            best_state = next_state;
        }
    }
    
    return best_state;
}
#include "hillclimb.h"
#include "../../../common/game_util.h"
#include <random>

// 힐클라임 배치 알고리즘 구현
AutoMazeState hillClimbPlacement(const AutoMazeState &state, int number) 
{
    AutoMazeState now_state = state;
    for (int character_id = 0; character_id < GameConstants::AutoMaze::CHARACTER_N; character_id++)
    {
        int y = GameUtil::mt_for_action() % GameConstants::Board::H;
        int x = GameUtil::mt_for_action() % GameConstants::Board::W;
        now_state.setCharacter(character_id, y, x);
    }
    
    ScoreType best_score = now_state.getScore();
    
    // 힐클라임 반복
    for (int i = 0; i < number; i++)
    {
        // 상태 복사
        auto next_state = now_state;
        
        int character_id = GameUtil::mt_for_action() % GameConstants::AutoMaze::CHARACTER_N;
        int y = GameUtil::mt_for_action() % GameConstants::Board::H;
        int x = GameUtil::mt_for_action() % GameConstants::Board::W;
        next_state.setCharacter(character_id, y, x);
        
        auto next_score = next_state.getScore();
        if (next_score > best_score)
        {
            best_score = next_score;
            now_state = next_state;
        }
    }
    
    return now_state;
}
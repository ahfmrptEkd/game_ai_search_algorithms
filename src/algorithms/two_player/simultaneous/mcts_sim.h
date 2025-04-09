#ifndef SIM_MCTS_SIM_H
#define SIM_MCTS_SIM_H

#include "../../../games/simmaze/simmaze_state.h"
#include "../../../common/game_util.h"
#include "random.h"
#include <vector>

namespace sim_mcts {
    // 교차 게임으로 변환한 상태 클래스
    // 순서가 차례로 돌아온다는 전제로 탐색하기 때문에 교대로 시뮬레이션하는 클래스를 별도 준비
    class AlternateMazeState {
    private:
        static constexpr const int END_TURN_MULTIPLIER = 2; // 동시 게임 1턴 = 교차 게임 2턴
        
        std::vector<std::vector<int>> points_; 
        int turn_; 
        struct Character {
            int y_;
            int x_;
            int game_score_;
            Character(const int y = 0, const int x = 0) : y_(y), x_(x), game_score_(0) {}
        };
        std::vector<Character> players_;
        
    public:
        // 동시 게임 상태를 교차 게임 상태로 변환
        AlternateMazeState(const SimMazeState& base_state, const int player_id);
        
        WinningStatus getWinningStatus() const;
        
        bool isDone() const;
        
        void advance(const int action);
        
        std::vector<int> legalActions() const;
    };
    
    class Node {
    private:
        AlternateMazeState state_;
        double w_;  
        
    public:
        std::vector<Node> child_nodes;
        double n_; 
        
        Node(const AlternateMazeState& state);
        
        double evaluate();
        
        void expand();
        
        Node& nextChildNode();
    };
    
    // UCB1 상수
    constexpr const double C = 1.0;
    
    // 노드 확장 임계값
    constexpr const int EXPAND_THRESHOLD = 10;
    
    int randomAction(const AlternateMazeState& state);
    
    double playout(AlternateMazeState* state);
    
    int mctsAction(const SimMazeState& state, const int player_id, const int playout_number);
}

int mctsSimSearchAction(const SimMazeState& state, const int player_id, int simulation_number);

#endif // SIM_MCTS_SIM_H
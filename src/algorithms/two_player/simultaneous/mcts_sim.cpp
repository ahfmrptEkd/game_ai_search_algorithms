#include "mcts_sim.h"
#include <cmath>
#include <cassert>
#include <algorithm>
#include <iostream>

namespace sim_mcts {
    
    AlternateMazeState::AlternateMazeState(const SimMazeState& base_state, const int player_id) {
        // 포인트 맵 복사
        points_.resize(GameConstants::Board::H, std::vector<int>(GameConstants::Board::W));
        for (int y = 0; y < GameConstants::Board::H; y++) {
            for (int x = 0; x < GameConstants::Board::W; x++) {
                points_[y][x] = base_state.getPoint(y, x);
            }
        }
        
        // 턴 설정 (동시 게임 1턴 = 교차 게임 2턴)
        turn_ = base_state.getTurn() * END_TURN_MULTIPLIER;
        
        // 플레이어 설정 (자신이 항상 플레이어 0이 되도록)
        players_.resize(2);
        if (player_id == 0) {
            players_[0] = Character(base_state.getPlayerY(0), base_state.getPlayerX(0));
            players_[0].game_score_ = base_state.getPlayerScore(0);
            
            players_[1] = Character(base_state.getPlayerY(1), base_state.getPlayerX(1));
            players_[1].game_score_ = base_state.getPlayerScore(1);
        } else {
            // 플레이어 순서 반전
            players_[0] = Character(base_state.getPlayerY(1), base_state.getPlayerX(1));
            players_[0].game_score_ = base_state.getPlayerScore(1);
            
            players_[1] = Character(base_state.getPlayerY(0), base_state.getPlayerX(0));
            players_[1].game_score_ = base_state.getPlayerScore(0);
        }
    }
    
    WinningStatus AlternateMazeState::getWinningStatus() const {
        if (isDone()) {
            if (players_[0].game_score_ > players_[1].game_score_)
                return WinningStatus::WIN;
            else if (players_[0].game_score_ < players_[1].game_score_)
                return WinningStatus::LOSE;
            else
                return WinningStatus::DRAW;
        }
        return WinningStatus::NONE;
    }
    
    bool AlternateMazeState::isDone() const {
        return turn_ >= GameConstants::Board::END_TURN * END_TURN_MULTIPLIER;
    }
    
    void AlternateMazeState::advance(const int action) {
        auto& player = players_[0];
        player.y_ += GameConstants::DY[action];
        player.x_ += GameConstants::DX[action];
        
        auto& point = points_[player.y_][player.x_];
        if (point > 0) {
            player.game_score_ += point;
            point = 0;
        }
        
        // 턴 증가 및 플레이어 교체
        turn_++;
        std::swap(players_[0], players_[1]);
    }
    
    std::vector<int> AlternateMazeState::legalActions() const {
        std::vector<int> actions;
        const auto& player = players_[0];
        
        for (int action = 0; action < 4; action++) {
            int ty = player.y_ + GameConstants::DY[action];
            int tx = player.x_ + GameConstants::DX[action];
            
            if (ty >= 0 && ty < GameConstants::Board::H && 
                tx >= 0 && tx < GameConstants::Board::W) {
                actions.emplace_back(action);
            }
        }
        return actions;
    }
    
    Node::Node(const AlternateMazeState& state) : state_(state), w_(0), n_(0) {}
    
    double Node::evaluate() {
        if (state_.isDone()) {
            double value = 0.5;
            WinningStatus status = state_.getWinningStatus();
            
            switch (status) {
                case WinningStatus::WIN:
                    value = 1.0;
                    break;
                case WinningStatus::LOSE:
                    value = 0.0;
                    break;
                case WinningStatus::DRAW:
                    value = 0.5;
                    break;
                default:
                    break;
            }
            
            w_ += value;
            n_++;
            return value;
        }
        
        // 자식 노드가 없으면 플레이아웃 수행
        if (child_nodes.empty()) {
            AlternateMazeState state_copy = state_;
            double value = playout(&state_copy);
            
            w_ += value;
            n_++;
            
            // 확장 임계치에 도달하면 노드 확장
            if (n_ == EXPAND_THRESHOLD) {
                expand();
            }
            
            return value;
        } 
        // 자식 노드가 있으면 다음 노드 평가
        else {

            // 상대 관점에서 게임을 봐야하므로 1-value 반환
            double value = 1.0 - nextChildNode().evaluate();
            w_ += value;
            n_++;
            return value;
        }
    }
    
    void Node::expand() {
        auto legal_actions = state_.legalActions();
        child_nodes.clear();
        
        for (const auto action : legal_actions) {
            child_nodes.emplace_back(state_);
            child_nodes.back().state_.advance(action);
        }
    }
    
    Node& Node::nextChildNode() {
        // 방문하지 않은 노드가 있으면 그 노드 선택
        for (auto& child_node : child_nodes) {
            if (child_node.n_ == 0) {
                return child_node;
            }
        }
        
        // 모든 노드를 방문했으면 UCB1 값으로 선택
        double t = 0.0;
        for (const auto& child_node : child_nodes) {
            t += child_node.n_;
        }
        
        double best_value = -GameConstants::INF;
        int best_action_index = 0;
        
        for (size_t i = 0; i < child_nodes.size(); i++) {
            const auto& child_node = child_nodes[i];

            // UCB1 계산에서 1-w/n은 현재 플레이어 관점의 승률로 변환하기 위함
            double ucb1_value = 1.0 - child_node.w_ / child_node.n_ + 
                               C * std::sqrt(2.0 * std::log(t) / child_node.n_);
            
            if (ucb1_value > best_value) {
                best_action_index = i;
                best_value = ucb1_value;
            }
        }
        
        return child_nodes[best_action_index];
    }
    
    int randomAction(const AlternateMazeState& state) {
        auto legal_actions = state.legalActions();
        if (legal_actions.empty()) {
            return 0;
        }
        return legal_actions[GameUtil::mt_for_action() % legal_actions.size()];
    }
    
    double playout(AlternateMazeState* state) {
        WinningStatus status = state->getWinningStatus();
        
        switch (status) {
            case WinningStatus::WIN:
                return 1.0;
            case WinningStatus::LOSE:
                return 0.0;
            case WinningStatus::DRAW:
                return 0.5;
            default:
                state->advance(randomAction(*state));
                // 상대방 관점에서 게임 진행 후 결과 반환
                return 1.0 - playout(state);
        }
    }
    
    int mctsAction(const SimMazeState& base_state, const int player_id, const int playout_number) {
        auto state = AlternateMazeState(base_state, player_id);
        
        Node root_node = Node(state);
        root_node.expand();
        
        for (int i = 0; i < playout_number; i++) {
            root_node.evaluate();
        }
        
        auto legal_actions = state.legalActions();
        int best_action_searched_number = -1;
        int best_action_index = -1;
        
        assert(legal_actions.size() == root_node.child_nodes.size());
        
        for (size_t i = 0; i < legal_actions.size(); i++) {
            int n = root_node.child_nodes[i].n_;
            if (n > best_action_searched_number) {
                best_action_index = i;
                best_action_searched_number = n;
            }
        }
        
        return legal_actions[best_action_index];
    }
}

int mctsSimSearchAction(const SimMazeState& state, const int player_id, int simulation_number) {
    return sim_mcts::mctsAction(state, player_id, simulation_number);
}
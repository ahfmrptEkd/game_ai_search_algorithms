#include "duct.h"
#include "../../games/simmaze/simmaze_state.h"
#include <vector>
#include <iostream>
#include <cassert>
#include <algorithm>


namespace sim_duct {
    // 플레이어 0 시점에서 노드 평가
    double playout(SimMazeState* state) {
        if (state->isDone()) {
            WinningStatus status = state->getWinningStatus();
            switch (status) {
                case WinningStatus::WIN:
                    return 1.0;
                case WinningStatus::LOSE:
                    return 0.0;
                case WinningStatus::DRAW:
                    return 0.5;
                default:
                    return 0.5;
            }
        }

        int action0 = simMazeRandomAction(*state, 0);
        int action1 = simMazeRandomAction(*state, 1);
        state->advance(action0, action1);

        return playout(state);
    }
    
    Node::Node(const SimMazeState& state) : state_(state), w_(0), n_(0) {}

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
                default:
                    break;
            }
            
            w_ += value;
            n_++;
            return value;
        }

        if (child_nodeses_.empty()) {
            SimMazeState state_copy = state_;
            double value = playout(&state_copy);
            
            w_ += value;
            n_++;
            
            // 임계값에 도달하면 노드 확장
            if (n_ == EXPAND_THRESHOLD) {
                expand();
            }
            
            return value;
        } 
        else {
            // 자식 노드가 있으면 다음 노드 평가
            double value = nextChildNode().evaluate();
            w_ += value;
            n_++;
            return value;
        }
    }

    void Node::expand() {
        auto legal_actions0 = state_.legalActions(0);
        auto legal_actions1 = state_.legalActions(1);
        
        child_nodeses_.clear();
        child_nodeses_.resize(legal_actions0.size());
        
        // 모든 가능한 액션 조합에 대해 자식 노드 생성
        for (size_t i = 0; i < legal_actions0.size(); i++) {
            child_nodeses_[i].resize(legal_actions1.size());
            
            for (size_t j = 0; j < legal_actions1.size(); j++) {
                child_nodeses_[i][j] = Node(state_);
                auto& child_node = child_nodeses_[i][j];
                
                // 이 액션 조합으로 게임 진행
                child_node.state_.advance(legal_actions0[i], legal_actions1[j]);
            }
        }
    }

    Node& Node::nextChildNode() {
        // 방문하지 않은 노드가 있으면 그 노드 선택
        for (auto& child_nodes : child_nodeses_) {
            for (auto& child_node : child_nodes) {
                if (child_node.n_ == 0) {
                    return child_node;
                }
            }
        }

        // 모든 노드 방문 시 DUCT 전략으로 선택
        double t = 0;
        for (auto& child_nodes : child_nodeses_) {
            for (auto& child_node : child_nodes) {
                t += child_node.n_;
            }
        }
        
        // 플레이어 0의 최적 행동 인덱스
        int best_is[2] = {-1, -1};

        double best_value = -GameConstants::INF;
        for (size_t i = 0; i < child_nodeses_.size(); i++) {
            const auto& child_nodes = child_nodeses_[i];
            double w = 0;
            double n = 0;

            // i 번째 행동의 모든 결과값 합산
            for (size_t j = 0; j < child_nodes.size(); j++) {
                const auto& child_node = child_nodes[j];    // 행 의 원소
                w += child_node.w_;
                n += child_node.n_;
            }

            //UCB1 값 계산
            double ucb1_value = w / n + C * std::sqrt(2.0 * std::log(t) / n );
            if (ucb1_value > best_value) {
                best_is[0] = i;
                best_value = ucb1_value;
            }
        }

        // 플레이어 1의 최적 행동 인덱스
        best_value = -GameConstants::INF;
        for (size_t j = 0; j < child_nodeses_[0].size(); j++) {
            double w = 0;
            double n = 0;

            // j 번째 행동의 모든 결과값 합산
            for (size_t i = 0; i < child_nodeses_.size(); i++) {
                const auto& child_node = child_nodeses_[i][j];
                w += child_node.w_;
                n += child_node.n_;
            }

            // 플레이어 1은 목표가 반대이므로 승률 반전
            w = n - w;

            double ucb1_value = w / n + C * std::sqrt(2.0 * std::log(t) / n);
            if (ucb1_value > best_value) {
                best_is[1] = j;
                best_value = ucb1_value;
            }
        }
        return child_nodeses_[best_is[0]][best_is[1]];
    }

    int ductAction(const SimMazeState& state, const int player_id, const int simulation_number) {
        Node root_node = Node(state);
        root_node.expand();
        
        // 지정된 횟수만큼 시뮬레이션
        for (int i = 0; i < simulation_number; i++) {
            root_node.evaluate();
        }
        
        auto legal_actions = state.legalActions(player_id);
        int i_size = root_node.child_nodeses_.size();
        int j_size = root_node.child_nodeses_[0].size();
        
        // 플레이어별 최적 행동 선택
        if (player_id == 0) {
            int best_action_searched_number = -1;
            int best_action_index = -1;
            
            for (int i = 0; i < i_size; i++) {
                int n = 0;
                for (int j = 0; j < j_size; j++) {
                    n += root_node.child_nodeses_[i][j].n_;
                }
                
                if (n > best_action_searched_number) {
                    best_action_index = i;
                    best_action_searched_number = n;
                }
            }
            
            return legal_actions[best_action_index];
        } 
        else {
            int best_action_searched_number = -1;
            int best_action_index = -1;
            
            for (int j = 0; j < j_size; j++) {
                int n = 0;
                for (int i = 0; i < i_size; i++) {
                    n += root_node.child_nodeses_[i][j].n_;
                }
                
                if (n > best_action_searched_number) {
                    best_action_index = j;
                    best_action_searched_number = n;
                }
            }
            
            return legal_actions[best_action_index];
        }
    }
}

int ductSearchAction(const SimMazeState& state, const int player_id, int simulation_number) {
    return sim_duct::ductAction(state, player_id, simulation_number);
}
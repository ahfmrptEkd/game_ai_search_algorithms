#include "thunder.h"
#include "../../../common/game_util.h"
#include <vector>
#include <cmath>
#include <cassert>
#include <iostream>

namespace thunder {
    Node::Node(const TwoMazeState& state) : state_(state), w_(0), n_(0) {}
    
    double Node::evaluate() {
        if (this->state_.isDone()) {
            double value = 0.0;
            WinningStatus status = this->state_.getWinningStatus();
            switch (status) {
                case (WinningStatus::WIN):
                    value = 1.0;
                    break;
                case (WinningStatus::LOSE):
                    value = 0.0;
                    break;
                case (WinningStatus::DRAW):
                    value = 0.5;
                    break;
                default:
                    break;
            }
            this->w_ += value;
            ++this->n_;
            return value;
        }
        
        // 자식 노드가 존재하지 않는 경우
        if (this->child_nodes_.empty()) {
            // 주요 차이점: 플레이아웃 대신 게임판 평가 함수 사용
            double value = this->state_.getScoreRate();
            this->w_ += value;
            ++this->n_;
            
            // 즉시 노드 확장 (임계치 없음)
            this->expand();
            
            return value;
        }
        else {
            double value = 1.0 - this->nextChildNode().evaluate();
            this->w_ += value;
            ++this->n_;
            return value;
        }
    }
    
    void Node::expand() {
        auto legal_actions = this->state_.legalActions();
        this->child_nodes_.clear();
        
        // 가능한 모든 액션에 대해 자식 노드 생성
        for (const auto action : legal_actions) {
            this->child_nodes_.emplace_back(this->state_);
            this->child_nodes_.back().state_.progress(action);
        }
    }
    
    // 다음 방문할 자식 노드 선택 (Thunder 알고리즘의 핵심)
    Node& Node::nextChildNode() {
        // 방문하지 않은 자식 노드가 있으면 해당 노드 선택
        for (auto& child_node : this->child_nodes_) {
            if (child_node.n_ == 0)
                return child_node;
        }
        
        // Thunder 알고리즘의 핵심: UCB1 대신 단순 승률 기반 선택
        double best_value = -GameConstants::INF;
        int best_action_index = -1;
        
        for (int i = 0; i < this->child_nodes_.size(); i++) {
            const auto& child_node = this->child_nodes_[i];
            
            // 1 - w/n은 상대방 입장에서 본 패배율 = 내 입장에서 본 승률
            double thunder_value = 1.0 - child_node.w_ / child_node.n_;
            
            if (thunder_value > best_value) {
                best_action_index = i;
                best_value = thunder_value;
            }
        }
        
        return this->child_nodes_[best_action_index];
    }
    
    int thunderSearchAction(const TwoMazeState& state, int simulation_number) {
        auto legal_actions = state.legalActions();
        if (legal_actions.empty()) {
            return -1;
        }
        
        Node root_node = Node(state);
        root_node.expand();
        
        for (int i = 0; i < simulation_number; i++) {
            root_node.evaluate();
        }
        
        int best_action_searched_number = -1;
        int best_action_index = -1;
        
        assert(legal_actions.size() == root_node.child_nodes_.size());
        
        for (int i = 0; i < legal_actions.size(); i++) {
            int n = root_node.child_nodes_[i].n_;
            if (n > best_action_searched_number) {
                best_action_index = i;
                best_action_searched_number = n;
            }
        }
        
        return legal_actions[best_action_index];
    }

    int thunderSearchActionWithTime(const TwoMazeState& state, const int64_t time_ms) {
        auto legal_actions = state.legalActions();
        if (legal_actions.empty()){
            return -1;
        }        

        Node root_node = Node(state);
        root_node.expand();

        TimeKeeper time_keeper(time_ms);

        // 시간이 남아 있는 동안 최대한 많은 시뮬레이션 수행
        int simulation_count = 0;
        while (!time_keeper.isTimeOver()) {
            root_node.evaluate();
            simulation_count++;
        }

        int best_action_searched_number = -1;
        int best_action_index = -1;

        assert(legal_actions.size() == root_node.child_nodes_.size());

        for (int i = 0; i < legal_actions.size(); i++) {
            int n = root_node.child_nodes_[i].n_;
            if (n > best_action_searched_number) {
                best_action_index = i;
                best_action_searched_number = n;
            }
        }
        
        return legal_actions[best_action_index];
    }
}

int thunderSearchAction(const TwoMazeState& state, int simulation_number) {
    return thunder::thunderSearchAction(state, simulation_number);
}

int thunderSearchActionWithTime(const TwoMazeState& state, const int64_t time_ms) {
    return thunder::thunderSearchActionWithTime(state, time_ms);
}
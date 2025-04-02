#include "mcts.h"
#include "../../../common/game_util.h"
#include "mc.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <cassert>

namespace mcts {
    Node::Node(const TwoMazeState& state) : state_(state), w_(0), n_(0), children_() {}

    double Node::evaluate() {
        // 게임 종료 시
        if (this->state_.isDone()) {
            double value = 0.5;
            WinningStatus status = this->state_.getWinningStatus();
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
            this->w_ += value;
            ++this->n_;
            return value;
        }

        // 자식 노드가 없으면 랜덤 플레이아웃
        if (this->children_.empty()) {
            TwoMazeState state_copy = this->state_;
            double value = monte_carlo::playout(state_copy); 
            this->w_ += value;
            ++this->n_;

            if (this->n_ == EXPAND_THRESHOLD) {
                this->expand();
            }

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
        this->children_.clear();

        for (const auto& action : legal_actions) {
            this->children_.emplace_back(this->state_);
            this->children_.back().state_.progress(action);
        }
    }

    Node& Node::nextChildNode() {
        for (auto& child : this->children_) {
            if (child.n_ == 0) {
                return child;
            }
        }

        double t = 0.0; // 모든 방문 횟수 집계
        for (const auto& child : this->children_) {
            t += child.n_;
        }

        double best_value = -GameConstants::INF;
        int best_action_index = 0;

        for (int i = 0; i < this->children_.size(); i++) {
            const auto& child_node = this->children_[i];
            
            // UCB1 공식 : exploitation(현재 가치) + C * sqrt( log(부모방문횟수) / 자식방문횟수)
            double ucb1_value = 1.0 - child_node.w_ / child_node.n_ + C * std::sqrt(std::log(t) / child_node.n_);

            if (ucb1_value > best_value) {
                best_value = ucb1_value;
                best_action_index = i;
            }
        }

        return this->children_[best_action_index];
    }

    int mctsAction(const TwoMazeState& state, int playout_number) {
        auto legal_actions = state.legalActions();
        if (legal_actions.empty()) {
            return -1;
        }

        Node root_node = Node(state);
        root_node.expand();

        for (int i = 0; i < playout_number; i++) {
            root_node.evaluate();
        }

        int best_action_searched_number = -1;
        int best_action_index = -1;

        assert(legal_actions.size() == root_node.children_.size());

        for (int i = 0; i < legal_actions.size(); i++) {
            int n = root_node.children_[i].n_;

            if (n > best_action_searched_number) {
                best_action_searched_number = n;
                best_action_index = i;
            }
        }

        return legal_actions[best_action_index];
    }
}

int mctsSearchAction(const TwoMazeState& state, int playout_number) {
    return mcts::mctsAction(state, playout_number);
}
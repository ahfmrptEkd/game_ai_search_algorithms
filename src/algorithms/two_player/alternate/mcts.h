#ifndef MCTS_H
#define MCTS_H

#include "../../../games/twomaze/twomaze_state.h"
#include "../../../common/game_util.h"
#include "mc.h"

namespace mcts {
    constexpr const double C = 1.0; // UCB1 계산에 사용하는 상수
    constexpr const int EXPAND_THRESHOLD = 10; // 노드 확장 임계치

    class Node {
    private:
        TwoMazeState state_;
        double w_; // 누적 가치

    public:
        std::vector<Node> children_;
        double n_;

        Node(const TwoMazeState& state);

        double evaluate();

        void expand();

        // 어떤 노드를 평가할지 선택 (UCB1 기반)
        Node& nextChildNode();

    };

    int mctsAction(const TwoMazeState& state, const int playout_number);
}

int mctsSearchAction(const TwoMazeState& state, const int playout_number);

#endif
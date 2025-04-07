#include "mcts.h"
#include <limits>

// Node 클래스 구현
ConnectFourMCTSAlgorithm::Node::Node(const ConnectFourState& state) : state_(state), w_(0), n_(0) {}

double ConnectFourMCTSAlgorithm::Node::evaluate() {
    if (this->state_.isDone()) {
        double value = 0.5;
        switch (this->state_.getWinningStatus()) {
            case (WinningStatus::WIN):
                value = 1.;
                break;
            case (WinningStatus::LOSE):
                value = 0.;
                break;
            default:
                break;
        }
        this->w_ += value;
        ++this->n_;
        return value;
    }
    
    if (this->child_nodes_.empty()) {
        ConnectFourState state_copy = this->state_;
        double value = ConnectFourMCTSAlgorithm::playout(&state_copy);
        this->w_ += value;
        ++this->n_;
        
        if (this->n_ == EXPAND_THRESHOLD)
            this->expand();
            
        return value;
    }
    else {
        double value = 1. - this->nextChildNode().evaluate();
        this->w_ += value;
        ++this->n_;
        return value;
    }
}

void ConnectFourMCTSAlgorithm::Node::expand() {
    auto legal_actions = this->state_.legalActions();
    this->child_nodes_.clear();
    for (const auto action : legal_actions) {
        this->child_nodes_.emplace_back(this->state_);
        this->child_nodes_.back().state_.progress(action);
    }
}

ConnectFourMCTSAlgorithm::Node& ConnectFourMCTSAlgorithm::Node::nextChildNode() {
    for (auto& child_node : this->child_nodes_) {
        if (child_node.n_ == 0)
            return child_node;
    }
    
    double t = 0;
    for (const auto& child_node : this->child_nodes_) {
        t += child_node.n_;
    }
    
    double best_value = -GameConstants::INF;
    int best_action_index = -1;
    
    for (int i = 0; i < this->child_nodes_.size(); i++) {
        const auto& child_node = this->child_nodes_[i];
        double ucb1_value = 1. - child_node.w_ / child_node.n_ + 
                          C * std::sqrt(2. * std::log(t) / child_node.n_);
                          
        if (ucb1_value > best_value) {
            best_action_index = i;
            best_value = ucb1_value;
        }
    }
    
    return this->child_nodes_[best_action_index];
}

// playout 함수 구현
double ConnectFourMCTSAlgorithm::playout(ConnectFourState* state) {
    switch (state->getWinningStatus()) {
        case (WinningStatus::WIN):
            return 1.;
        case (WinningStatus::LOSE):
            return 0.;
        case (WinningStatus::DRAW):
            return 0.5;
        default:
            auto legal_actions = state->legalActions();
            state->progress(legal_actions[GameUtil::mt_for_action() % legal_actions.size()]);
            return 1. - playout(state);
    }
}

// Algorithm 인터페이스 구현
int ConnectFourMCTSAlgorithm::selectAction(const GameState& state) {
    auto& connectfour_state = static_cast<const ConnectFourState&>(state);
    
    Node root_node = Node(connectfour_state);
    root_node.expand();
    
    TimeKeeper time_keeper(params_.timeThreshold);
    
    while (!time_keeper.isTimeOver()) {
        root_node.evaluate();
    }
    
    auto legal_actions = connectfour_state.legalActions();
    if (legal_actions.empty()) {
        return -1;
    }
    
    int best_action_searched_number = -1;
    int best_action_index = -1;
    
    for (int i = 0; i < legal_actions.size(); i++) {
        int n = root_node.child_nodes_[i].n_;
        if (n > best_action_searched_number) {
            best_action_index = i;
            best_action_searched_number = n;
        }
    }
    
    return legal_actions[best_action_index];
}

std::string ConnectFourMCTSAlgorithm::getName() const {
    return "MCTS (ConnectFour)";
}

void ConnectFourMCTSAlgorithm::setParams(const AlgorithmParams& params) {
    params_ = params;
}

std::unique_ptr<GameState> ConnectFourMCTSAlgorithm::runAndEvaluate(const GameState& state, int action) {
    auto connectfour_state = static_cast<const ConnectFourState&>(state);
    auto next_state = std::make_unique<ConnectFourState>(connectfour_state);
    next_state->progress(action);
    return next_state;
} 
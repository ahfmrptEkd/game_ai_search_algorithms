#include "random.h"

ConnectFourRandomAlgorithm::ConnectFourRandomAlgorithm() : random_engine_(std::random_device{}()) {}

int ConnectFourRandomAlgorithm::selectAction(const GameState& state) {
    auto connectfour_state = static_cast<const ConnectFourState&>(state);
    auto legal_actions = connectfour_state.legalActions();
    if (legal_actions.empty()) {
        throw std::runtime_error("No legal actions available");
    }
    return legal_actions[random_engine_() % legal_actions.size()];
}

std::string ConnectFourRandomAlgorithm::getName() const {
    return "Random (Connect Four)";
}

void ConnectFourRandomAlgorithm::setParams(const AlgorithmParams& params) {
    params_ = params;
}

std::unique_ptr<GameState> ConnectFourRandomAlgorithm::runAndEvaluate(const GameState& state, int action) {
    auto connectfour_state = static_cast<const ConnectFourState&>(state);
    auto next_state = std::make_unique<ConnectFourState>(connectfour_state);
    next_state->progress(action);
    return next_state;
} 
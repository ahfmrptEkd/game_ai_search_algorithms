#ifndef CONNECT_FOUR_RANDOM_H
#define CONNECT_FOUR_RANDOM_H

#include "../algorithm_interface.h"
#include "../../games/connect_four/connect_four_state.h"
#include <random>

class ConnectFourRandomAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    std::mt19937 random_engine_;
    
public:
    ConnectFourRandomAlgorithm();
    int selectAction(const GameState& state) override;
    std::string getName() const override;
    void setParams(const AlgorithmParams& params) override;
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override;
};

#endif // CONNECT_FOUR_RANDOM_H 
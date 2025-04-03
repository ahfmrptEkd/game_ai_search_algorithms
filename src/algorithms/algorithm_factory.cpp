#include "algorithm_interface.h"
#include "single_player/without_context/hillclimb.h"
#include "single_player/without_context/simulated_annealing.h"
#include "single_player/without_context/random.h"
#include <stdexcept>

// 힐클라임 알고리즘 구현
class HillClimbAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    
public:
    int selectAction(const GameState& state) override {
        // 기존 힐클라임 알고리즘을 인터페이스에 맞게 래핑
        // 실제 구현에서는 구체적인 상태 타입으로 캐스팅 필요
        return 0; // 임시 구현
    }
    
    std::string getName() const override {
        return "HillClimb";
    }
    
    void setParams(const AlgorithmParams& params) override {
        params_ = params;
    }
    
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override {
        auto cloned_state = state.clone();
        cloned_state->progress(action);
        return cloned_state;
    }
};

// 시뮬레이티드 어닐링 알고리즘 구현
class SimulatedAnnealingAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    
public:
    int selectAction(const GameState& state) override {
        // 기존 시뮬레이티드 어닐링 알고리즘을 인터페이스에 맞게 래핑
        return 0; // 임시 구현
    }
    
    std::string getName() const override {
        return "SimulatedAnnealing";
    }
    
    void setParams(const AlgorithmParams& params) override {
        params_ = params;
    }
    
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override {
        auto cloned_state = state.clone();
        cloned_state->progress(action);
        return cloned_state;
    }
};

// 랜덤 알고리즘 구현
class RandomAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    
public:
    int selectAction(const GameState& state) override {
        // 기존 랜덤 알고리즘을 인터페이스에 맞게 래핑
        auto actions = state.legalActions();
        if (actions.empty()) {
            return -1; // 유효한 행동이 없는 경우
        }
        return actions[rand() % actions.size()];
    }
    
    std::string getName() const override {
        return "Random";
    }
    
    void setParams(const AlgorithmParams& params) override {
        params_ = params;
    }
    
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override {
        auto cloned_state = state.clone();
        cloned_state->progress(action);
        return cloned_state;
    }
};

// 알고리즘 팩토리 구현
std::unique_ptr<Algorithm> AlgorithmFactory::createAlgorithm(
    const std::string& algorithmName, const AlgorithmParams& params) {
    
    std::unique_ptr<Algorithm> algorithm;
    
    if (algorithmName == "HillClimb") {
        algorithm = std::make_unique<HillClimbAlgorithm>();
    } else if (algorithmName == "SimulatedAnnealing") {
        algorithm = std::make_unique<SimulatedAnnealingAlgorithm>();
    } else if (algorithmName == "Random") {
        algorithm = std::make_unique<RandomAlgorithm>();
    } else {
        throw std::invalid_argument("Unknown algorithm: " + algorithmName);
    }
    
    algorithm->setParams(params);
    return algorithm;
} 
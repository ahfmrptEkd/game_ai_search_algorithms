#include "algorithm_interface.h"
#include "single_player/without_context/hillclimb.h"
#include "single_player/without_context/simulated_annealing.h"
#include "single_player/without_context/random.h"

#include "single_player/with_context/random.h"
#include "single_player/with_context/greedy.h"
#include "single_player/with_context/beam.h"
#include "single_player/with_context/chokudai.h"

#include "two_player/alternate/minimax.h"
#include "two_player/alternate/alphabeta.h"
#include "two_player/alternate/deepening.h"
#include "two_player/alternate/mc.h"
#include "two_player/alternate/mcts.h"
#include "two_player/alternate/thunder.h"

#include "two_player/simultaneous/random.h"
#include "two_player/simultaneous/pmc.h"
#include "two_player/simultaneous/mcts_sim.h"
#include "two_player/simultaneous/duct.h"

#include "../games/maze/maze_state.h"
#include "../games/automaze/automaze_state.h"
#include "../games/twomaze/twomaze_state.h"
#include "../games/simmaze/simmaze_state.h"

#include "../common/game_util.h"
#include <stdexcept>
#include <random>


// ----- single player, without context -----

// 랜덤 알고리즘 구현 (AutoMaze 용)
class AutoMazeRandomAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    
public:
    int selectAction(const GameState& state) override {
        // AutoMaze에서는 action이 없으므로 0을 반환
        return 0;
    }
    
    std::string getName() const override {
        return "Random (AutoMaze)";
    }
    
    void setParams(const AlgorithmParams& params) override {
        params_ = params;
    }
    
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override {
        // 상태를 복제하고 AutoMazeState로 캐스팅
        auto maze_state = static_cast<const AutoMazeState&>(state);
        auto next_state = std::make_unique<AutoMazeState>(maze_state);
        
        // 랜덤 위치에 캐릭터 배치
        for (int character_id = 0; character_id < GameConstants::AutoMaze::CHARACTER_N; character_id++) {
            int y = GameUtil::mt_for_action() % GameConstants::Board::H;
            int x = GameUtil::mt_for_action() % GameConstants::Board::W;
            next_state->setCharacter(character_id, y, x);
        }
        
        return next_state;
    }
};

class HillClimbAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    
public:
    int selectAction(const GameState& state) override {
        return 0;
    }
    
    std::string getName() const override {
        return "HillClimb";
    }
    
    void setParams(const AlgorithmParams& params) override {
        params_ = params;
    }
    
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override {
        auto maze_state = static_cast<const AutoMazeState&>(state);
        return std::make_unique<AutoMazeState>(
            hillClimbPlacement(maze_state, params_.searchNumber));
    }
};

class SimulatedAnnealingAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    
public:
    int selectAction(const GameState& state) override {
        return 0;
    }
    
    std::string getName() const override {
        return "SimulatedAnnealing";
    }
    
    void setParams(const AlgorithmParams& params) override {
        params_ = params;
    }
    
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override {
        auto maze_state = static_cast<const AutoMazeState&>(state);
        return std::make_unique<AutoMazeState>(
            simulatedAnnealingPlacement(maze_state, 
                                        params_.searchNumber, 
                                        params_.startTemperature,
                                        params_.endTemperature));
    }
};


// ----- single player, with context -----

class MazeRandomAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    
public:
    int selectAction(const GameState& state) override {
        auto maze_state = static_cast<const MazeState&>(state);
        return mazeRandomAction(maze_state);
    }
    
    std::string getName() const override {
        return "Random (Maze)";
    }
    
    void setParams(const AlgorithmParams& params) override {
        params_ = params;
    }
    
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override {
        auto maze_state = static_cast<const MazeState&>(state);
        auto next_state = std::make_unique<MazeState>(maze_state);
        next_state->progress(action);
        return next_state;
    }
};

class GreedyAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    
public:
    int selectAction(const GameState& state) override {
        auto maze_state = static_cast<const MazeState&>(state);
        return greedyAction(maze_state);
    }
    
    std::string getName() const override {
        return "Greedy";
    }
    
    void setParams(const AlgorithmParams& params) override {
        params_ = params;
    }
    
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override {
        auto maze_state = static_cast<const MazeState&>(state);
        auto next_state = std::make_unique<MazeState>(maze_state);
        next_state->progress(action);
        return next_state;
    }
};

class BeamSearchAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    
public:
    int selectAction(const GameState& state) override {
        auto maze_state = static_cast<const MazeState&>(state);
        
        BeamConfig config;
        config.search_width = params_.searchWidth;
        config.search_depth = params_.searchDepth;
        config.time_threshold = params_.timeThreshold;
        
        return beamSearchAction(maze_state, config);
    }
    
    std::string getName() const override {
        return "BeamSearch";
    }
    
    void setParams(const AlgorithmParams& params) override {
        params_ = params;
    }
    
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override {
        auto maze_state = static_cast<const MazeState&>(state);
        auto next_state = std::make_unique<MazeState>(maze_state);
        next_state->progress(action);
        return next_state;
    }
};

class ChokudaiAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    
public:
    int selectAction(const GameState& state) override {
        auto maze_state = static_cast<const MazeState&>(state);
        
        ChokudaiConfig config;
        config.search_width = params_.searchWidth;
        config.search_depth = params_.searchDepth;
        config.search_number = params_.searchNumber;
        config.time_threshold = params_.timeThreshold;
        
        return chokudaiSearchAction(maze_state, config);
    }
    
    std::string getName() const override {
        return "Chokudai";
    }
    
    void setParams(const AlgorithmParams& params) override {
        params_ = params;
    }
    
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override {
        auto maze_state = static_cast<const MazeState&>(state);
        auto next_state = std::make_unique<MazeState>(maze_state);
        next_state->progress(action);
        return next_state;
    }
};

// ----- two player, alternate -----
class TwoMazeRandomAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    
public:
    int selectAction(const GameState& state) override {
        auto maze_state = static_cast<const TwoMazeState&>(state);
        auto legal_actions = maze_state.legalActions();
        if (legal_actions.empty()) {
            return -1;
        }
        return legal_actions[GameUtil::mt_for_action() % legal_actions.size()];
    }
    
    std::string getName() const override {
        return "Random (TwoMaze)";
    }
    
    void setParams(const AlgorithmParams& params) override {
        params_ = params;
    }
    
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override {
        auto maze_state = static_cast<const TwoMazeState&>(state);
        auto next_state = std::make_unique<TwoMazeState>(maze_state);
        next_state->progress(action);
        return next_state;
    }
};

class MinimaxAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    
public:
    int selectAction(const GameState& state) override {
        auto maze_state = static_cast<const TwoMazeState&>(state);
        return miniMaxSearchAction(maze_state, params_.searchDepth);
    }
    
    std::string getName() const override {
        return "Minimax";
    }
    
    void setParams(const AlgorithmParams& params) override {
        params_ = params;
    }
    
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override {
        auto maze_state = static_cast<const TwoMazeState&>(state);
        auto next_state = std::make_unique<TwoMazeState>(maze_state);
        next_state->progress(action);
        return next_state;
    }
};

class AlphaBetaAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    
public:
    int selectAction(const GameState& state) override {
        auto maze_state = static_cast<const TwoMazeState&>(state);
        return alphaBetaSearchAction(maze_state, params_.searchDepth);
    }
    
    std::string getName() const override {
        return "AlphaBeta";
    }
    
    void setParams(const AlgorithmParams& params) override {
        params_ = params;
    }
    
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override {
        auto maze_state = static_cast<const TwoMazeState&>(state);
        auto next_state = std::make_unique<TwoMazeState>(maze_state);
        next_state->progress(action);
        return next_state;
    }
};

class IterativeDeepeningAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    
public:
    int selectAction(const GameState& state) override {
        auto maze_state = static_cast<const TwoMazeState&>(state);
        return iterativeDeepeningSearchAction(maze_state, params_.timeThreshold);
    }
    
    std::string getName() const override {
        return "IterativeDeepening";
    }
    
    void setParams(const AlgorithmParams& params) override {
        params_ = params;
    }
    
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override {
        auto maze_state = static_cast<const TwoMazeState&>(state);
        auto next_state = std::make_unique<TwoMazeState>(maze_state);
        next_state->progress(action);
        return next_state;
    }
};

class MonteCarloAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    
public:
    int selectAction(const GameState& state) override {
        auto maze_state = static_cast<const TwoMazeState&>(state);
        return monteCarloSearchAction(maze_state, params_.playoutNumber);
    }
    
    std::string getName() const override {
        return "MonteCarlo";
    }
    
    void setParams(const AlgorithmParams& params) override {
        params_ = params;
    }
    
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override {
        auto maze_state = static_cast<const TwoMazeState&>(state);
        auto next_state = std::make_unique<TwoMazeState>(maze_state);
        next_state->progress(action);
        return next_state;
    }
};

class MCTSAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    
public:
    int selectAction(const GameState& state) override {
        auto maze_state = static_cast<const TwoMazeState&>(state);
        return mctsSearchAction(maze_state, params_.playoutNumber);
    }
    
    std::string getName() const override {
        return "MCTS";
    }
    
    void setParams(const AlgorithmParams& params) override {
        params_ = params;
    }
    
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override {
        auto maze_state = static_cast<const TwoMazeState&>(state);
        auto next_state = std::make_unique<TwoMazeState>(maze_state);
        next_state->progress(action);
        return next_state;
    }
};

class ThunderAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    
public:
    int selectAction(const GameState& state) override {
        auto maze_state = static_cast<const TwoMazeState&>(state);
        return thunderSearchAction(maze_state, params_.playoutNumber);
    }
    
    std::string getName() const override {
        return "Thunder";
    }
    
    void setParams(const AlgorithmParams& params) override {
        params_ = params;
    }
    
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override {
        auto maze_state = static_cast<const TwoMazeState&>(state);
        auto next_state = std::make_unique<TwoMazeState>(maze_state);
        next_state->progress(action);
        return next_state;
    }
};

class ThunderTimeAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    
public:
    int selectAction(const GameState& state) override {
        auto maze_state = static_cast<const TwoMazeState&>(state);
        return thunderSearchActionWithTime(maze_state, params_.timeThreshold);
    }
    
    std::string getName() const override {
        return "ThunderTime";
    }
    
    void setParams(const AlgorithmParams& params) override {
        params_ = params;
    }
    
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override {
        auto maze_state = static_cast<const TwoMazeState&>(state);
        auto next_state = std::make_unique<TwoMazeState>(maze_state);
        next_state->progress(action);
        return next_state;
    }
};

// ----- two player, simultaneous -----
// SimMazeRandom 알고리즘 클래스
class SimMazeRandomAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    int player_id_ = 0;
    
public:
    int selectAction(const GameState& state) override {
        try {
            auto& maze_state = static_cast<const SimMazeState&>(state);
            return simMazeRandomAction(maze_state, player_id_);
        } catch (const std::bad_cast&) {
            throw std::runtime_error("SimMazeRandomAlgorithm expects SimMazeState");
        }
    }
    
    std::string getName() const override {
        return "Random (SimMaze)";
    }
    
    void setParams(const AlgorithmParams& params) override {
        params_ = params;
        player_id_ = params.playerId;
    }
    
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int encoded_action) override {
        try {
            auto& maze_state = static_cast<const SimMazeState&>(state);
            auto next_state = std::make_unique<SimMazeState>(maze_state);
            
            // encoded_action을 두 플레이어의 액션으로 디코딩
            auto actions = SimMazeState::decodeActions(encoded_action);
            next_state->advance(actions.first, actions.second);
            
            return next_state;
        } catch (const std::bad_cast&) {
            throw std::runtime_error("SimMazeRandomAlgorithm expects SimMazeState");
        }
    }
};

// DUCT 알고리즘 클래스
class DuctAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    int player_id_ = 0;
    
public:
    int selectAction(const GameState& state) override {
        try {
            auto& maze_state = static_cast<const SimMazeState&>(state);
            return ductSearchAction(maze_state, player_id_, params_.playoutNumber);
        } catch (const std::bad_cast&) {
            throw std::runtime_error("DuctAlgorithm expects SimMazeState");
        }
    }
    
    std::string getName() const override {
        return "DUCT (SimMaze)";
    }
    
    void setParams(const AlgorithmParams& params) override {
        params_ = params;
        player_id_ = params.playerId;
    }
    
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int encoded_action) override {
        try {
            auto& maze_state = static_cast<const SimMazeState&>(state);
            auto next_state = std::make_unique<SimMazeState>(maze_state);
            
            auto actions = SimMazeState::decodeActions(encoded_action);
            next_state->advance(actions.first, actions.second);
            
            return next_state;
        } catch (const std::bad_cast&) {
            throw std::runtime_error("DuctAlgorithm expects SimMazeState");
        }
    }
};

// Primitive Monte Carlo 알고리즘 클래스
class PMCAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    int player_id_ = 0;
    
public:
    int selectAction(const GameState& state) override {
        try {
            auto& maze_state = static_cast<const SimMazeState&>(state);
            return pmcSearchAction(maze_state, player_id_, params_.playoutNumber);
        } catch (const std::bad_cast&) {
            throw std::runtime_error("PMCAlgorithm expects SimMazeState");
        }
    }
    
    std::string getName() const override {
        return "Primitive Monte Carlo (SimMaze)";
    }
    
    void setParams(const AlgorithmParams& params) override {
        params_ = params;
        player_id_ = params.playerId;
    }
    
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int encoded_action) override {
        try {
            auto& maze_state = static_cast<const SimMazeState&>(state);
            auto next_state = std::make_unique<SimMazeState>(maze_state);
            
            auto actions = SimMazeState::decodeActions(encoded_action);
            next_state->advance(actions.first, actions.second);
            
            return next_state;
        } catch (const std::bad_cast&) {
            throw std::runtime_error("PMCAlgorithm expects SimMazeState");
        }
    }
};

// MCTS Simulation 알고리즘 클래스
class MCTSSimAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    int player_id_ = 0;
    
public:
    int selectAction(const GameState& state) override {
        try {
            auto& maze_state = static_cast<const SimMazeState&>(state);
            return mctsSimSearchAction(maze_state, player_id_, params_.playoutNumber);
        } catch (const std::bad_cast&) {
            throw std::runtime_error("MCTSSimAlgorithm expects SimMazeState");
        }
    }
    
    std::string getName() const override {
        return "MCTS Simulation (SimMaze)";
    }
    
    void setParams(const AlgorithmParams& params) override {
        params_ = params;
        player_id_ = params.playerId;
    }
    
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int encoded_action) override {
        try {
            auto& maze_state = static_cast<const SimMazeState&>(state);
            auto next_state = std::make_unique<SimMazeState>(maze_state);
            
            auto actions = SimMazeState::decodeActions(encoded_action);
            next_state->advance(actions.first, actions.second);
            
            return next_state;
        } catch (const std::bad_cast&) {
            throw std::runtime_error("MCTSSimAlgorithm expects SimMazeState");
        }
    }
};

// 알고리즘 팩토리 구현
std::unique_ptr<Algorithm> AlgorithmFactory::createAlgorithm(
    const std::string& algorithmName, const AlgorithmParams& params) {
    
    std::unique_ptr<Algorithm> algorithm;
    
    // 단일 플레이어 (컨텍스트 없음)
    if (algorithmName == "AutoMazeRandom") {
        algorithm = std::make_unique<AutoMazeRandomAlgorithm>();
    } else if (algorithmName == "HillClimb") {
        algorithm = std::make_unique<HillClimbAlgorithm>();
    } else if (algorithmName == "SimulatedAnnealing") {
        algorithm = std::make_unique<SimulatedAnnealingAlgorithm>();
    } 
    // 단일 플레이어 (컨텍스트 있음)
    else if (algorithmName == "MazeRandom") {
        algorithm = std::make_unique<MazeRandomAlgorithm>();
    } else if (algorithmName == "Greedy") {
        algorithm = std::make_unique<GreedyAlgorithm>();
    } else if (algorithmName == "BeamSearch") {
        algorithm = std::make_unique<BeamSearchAlgorithm>();
    } else if (algorithmName == "Chokudai") {
        algorithm = std::make_unique<ChokudaiAlgorithm>();
    } 
    // 두 플레이어 (교대 플레이)
    else if (algorithmName == "TwoMazeRandom") {
        algorithm = std::make_unique<TwoMazeRandomAlgorithm>();
    } else if (algorithmName == "Minimax") {
        algorithm = std::make_unique<MinimaxAlgorithm>();
    } else if (algorithmName == "AlphaBeta") {
        algorithm = std::make_unique<AlphaBetaAlgorithm>();
    } else if (algorithmName == "IterativeDeepening") {
        algorithm = std::make_unique<IterativeDeepeningAlgorithm>();
    } else if (algorithmName == "MonteCarlo") {
        algorithm = std::make_unique<MonteCarloAlgorithm>();
    } else if (algorithmName == "MCTS") {
        algorithm = std::make_unique<MCTSAlgorithm>();
    } else if (algorithmName == "Thunder") {
        algorithm = std::make_unique<ThunderAlgorithm>();
    } else if (algorithmName == "ThunderTime") {
        algorithm = std::make_unique<ThunderTimeAlgorithm>();
    }
    // 두 플레이어 (동시 게임)
    else if (algorithmName == "SimMazeRandom") {
        algorithm = std::make_unique<SimMazeRandomAlgorithm>();
    } else if (algorithmName == "SimMazeDUCT") {
        algorithm = std::make_unique<DuctAlgorithm>();
    } else if (algorithmName == "SimMazePMC") {
        algorithm = std::make_unique<PMCAlgorithm>();
    } else if (algorithmName == "SimMazeMCTS") {
        algorithm = std::make_unique<MCTSSimAlgorithm>();
    }
    else {
        throw std::invalid_argument("Unknown algorithm: " + algorithmName);
    }
    
    algorithm->setParams(params);
    return algorithm;
}
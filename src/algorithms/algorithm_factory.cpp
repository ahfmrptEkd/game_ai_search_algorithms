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

// connet four
#include "../games/connect_four/connect_four_state.h"
#include "../games/connect_four/connect_four_bitboard.h"

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

// ----- connect four -----
class ConnectFourRandomAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    
public:
    int selectAction(const GameState& state) override {
        auto connectfour_state = static_cast<const ConnectFourState&>(state);
        auto legal_actions = connectfour_state.legalActions();
        if (legal_actions.empty()) {
            return -1;
        }
        return legal_actions[GameUtil::mt_for_action() % legal_actions.size()];
    }
    
    std::string getName() const override {
        return "Random (ConnectFour)";
    }
    
    void setParams(const AlgorithmParams& params) override {
        params_ = params;
    }
    
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override {
        auto connectfour_state = static_cast<const ConnectFourState&>(state);
        auto next_state = std::make_unique<ConnectFourState>(connectfour_state);
        next_state->progress(action);
        return next_state;
    }
};

// ConnectFour MCTS 알고리즘 클래스
class ConnectFourMCTSAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    
    // MCTS 구현에 필요한 상수 및 유틸리티
    static constexpr const double C = 1.;
    static constexpr const int EXPAND_THRESHOLD = 10;
    
    static double playout(ConnectFourState* state) {
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
    
    // MCTS 노드 클래스
    class Node {
    private:
        ConnectFourState state_;
        double w_;
        
    public:
        std::vector<Node> child_nodes_;
        double n_;
        
        Node(const ConnectFourState& state) : state_(state), w_(0), n_(0) {}
        
        double evaluate() {
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
        
        void expand() {
            auto legal_actions = this->state_.legalActions();
            this->child_nodes_.clear();
            for (const auto action : legal_actions) {
                this->child_nodes_.emplace_back(this->state_);
                this->child_nodes_.back().state_.progress(action);
            }
        }
        
        Node& nextChildNode() {
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
    };
    
public:
    int selectAction(const GameState& state) override {
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
    
    std::string getName() const override {
        return "MCTS (ConnectFour)";
    }
    
    void setParams(const AlgorithmParams& params) override {
        params_ = params;
    }
    
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override {
        auto connectfour_state = static_cast<const ConnectFourState&>(state);
        auto next_state = std::make_unique<ConnectFourState>(connectfour_state);
        next_state->progress(action);
        return next_state;
    }
};

// ConnectFour 비트보드 MCTS 알고리즘 클래스
class ConnectFourBitMCTSAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    
    static double playout(ConnectFourBitBoardState* state) {
        switch (state->getWinningStatus()) {
            case (WinningStatus::WIN):
                return 1.;
            case (WinningStatus::LOSE):
                return 0.;
            case (WinningStatus::DRAW):
                return 0.5;
            default:
                auto legal_actions = state->legalActions();
                if (legal_actions.empty()) {
                    return 0.5; 
                }
                state->advance(legal_actions[GameUtil::mt_for_action() % legal_actions.size()]);
                return 1. - playout(state);
        }
    }
    
    // MCTS 노드 클래스 (비트보드 버전)
    class Node {
    private:
        ConnectFourBitBoardState state_;
        double w_;
        
    public:
        std::vector<Node> child_nodes_;
        double n_;
        
        static constexpr const double C = 1.;
        static constexpr const int EXPAND_THRESHOLD = 10;
        
        Node(const ConnectFourBitBoardState& state) : state_(state), w_(0), n_(0) {}
        
        double evaluate() {
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
                ConnectFourBitBoardState state_copy = this->state_;
                double value = ConnectFourBitMCTSAlgorithm::playout(&state_copy);
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
        
        void expand() {
            auto legal_actions = this->state_.legalActions();
            this->child_nodes_.clear();
            for (const auto action : legal_actions) {
                this->child_nodes_.emplace_back(this->state_);
                this->child_nodes_.back().state_.advance(action);
            }
        }
        
        Node& nextChildNode() {
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
    };
    
public:
    int selectAction(const GameState& state) override {
        auto& connectfour_state = static_cast<const ConnectFourState&>(state);
        ConnectFourBitBoardState bitboard_state(connectfour_state);
        
        Node root_node = Node(bitboard_state);
        root_node.expand();
        
        TimeKeeper time_keeper(params_.timeThreshold);
        
        while (!time_keeper.isTimeOver()) {
            root_node.evaluate();
        }
        
        auto legal_actions = bitboard_state.legalActions();
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
    
    std::string getName() const override {
        return "BitMCTS (ConnectFour)";
    }
    
    void setParams(const AlgorithmParams& params) override {
        params_ = params;
    }
    
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override {
        auto connectfour_state = static_cast<const ConnectFourState&>(state);
        auto next_state = std::make_unique<ConnectFourState>(connectfour_state);
        next_state->progress(action);
        return next_state;
    }
};

// ConnectFourIterativeDeepeningAlgorithm 클래스 추가
class ConnectFourIterativeDeepeningAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    std::mt19937 random_engine_;
    
    // 알파베타 프루닝을 사용한 미니맥스 알고리즘
    std::pair<int, double> alphabeta(const ConnectFourState& state, int depth, double alpha, double beta, bool maximizing_player) {
        // 게임이 종료되었거나 최대 깊이에 도달한 경우
        if (state.isDone() || depth == 0) {
            WinningStatus status = state.getWinningStatus();
            if (status == WinningStatus::WIN) {
                return {-1, maximizing_player ? 1.0 : -1.0};
            } else if (status == WinningStatus::LOSE) {
                return {-1, maximizing_player ? -1.0 : 1.0};
            } else if (status == WinningStatus::DRAW) {
                return {-1, 0.0};
            }
            
            // 평가 함수 사용 - const가 아니므로 복사본 생성
            ConnectFourState state_copy = state;
            return {-1, state_copy.evaluateScore() / 1000.0 * (maximizing_player ? 1.0 : -1.0)};
        }
        
        std::vector<int> legal_actions = state.legalActions();
        if (legal_actions.empty()) {
            return {-1, 0.0};
        }
        
        int best_action = legal_actions[0];
        double best_value = maximizing_player ? -std::numeric_limits<double>::infinity() : std::numeric_limits<double>::infinity();
        
        for (int action : legal_actions) {
            ConnectFourState next_state = ConnectFourState(state);
            next_state.progress(action);
            
            double value = alphabeta(next_state, depth - 1, alpha, beta, !maximizing_player).second;
            
            if (maximizing_player) {
                if (value > best_value) {
                    best_value = value;
                    best_action = action;
                }
                alpha = std::max(alpha, best_value);
            } else {
                if (value < best_value) {
                    best_value = value;
                    best_action = action;
                }
                beta = std::min(beta, best_value);
            }
            
            // 알파-베타 가지치기
            if (beta <= alpha) {
                break;
            }
        }
        
        return {best_action, best_value};
    }
    
    // Iterative Deepening 구현
    int iterativeDeepening(const ConnectFourState& state) {
        int best_action = -1;
        
        // 가능한 모든 행동 확인
        std::vector<int> legal_actions = state.legalActions();
        if (legal_actions.empty()) {
            return -1;
        }
        
        // 적어도 하나의 유효한 수는 반환
        best_action = legal_actions[random_engine_() % legal_actions.size()];
        
        auto start_time = std::chrono::high_resolution_clock::now();
        int max_depth = params_.searchDepth;
        
        // 시간 제한이 0이면 설정된 깊이까지만 탐색
        if (params_.timeThreshold <= 0) {
            return alphabeta(state, max_depth, -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), true).first;
        }
        
        // 시간 제한 내에서 점진적으로 깊이 증가
        for (int depth = 1; depth <= max_depth; depth++) {
            int current_action = alphabeta(state, depth, -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), true).first;
            
            // 유효한 행동을 찾았으면 업데이트
            if (current_action != -1) {
                best_action = current_action;
            }
            
            // 시간 확인
            auto current_time = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
            
            // 시간 제한에 근접했으면 현재까지의 최선의 수 반환
            if (elapsed >= params_.timeThreshold * 0.8) {
                break;
            }
        }
        
        return best_action;
    }
    
public:
    ConnectFourIterativeDeepeningAlgorithm() : random_engine_(std::random_device{}()) {}
    
    int selectAction(const GameState& state) override {
        auto connectfour_state = static_cast<const ConnectFourState&>(state);
        return iterativeDeepening(connectfour_state);
    }
    
    std::string getName() const override {
        return "Iterative Deepening (Connect Four)";
    }
    
    void setParams(const AlgorithmParams& params) override {
        params_ = params;
    }
    
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override {
        auto connectfour_state = static_cast<const ConnectFourState&>(state);
        auto next_state = std::make_unique<ConnectFourState>(connectfour_state);
        next_state->progress(action);
        return next_state;
    }
};

// ConnectFourBitIterativeDeepeningAlgorithm 클래스 추가
class ConnectFourBitIterativeDeepeningAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    std::mt19937 random_engine_;
    
    // 알파베타 프루닝을 사용한 미니맥스 알고리즘 (비트보드 버전)
    std::pair<int, double> alphabeta(const ConnectFourBitBoardState& state, int depth, double alpha, double beta, bool maximizing_player) {
        // 게임이 종료되었거나 최대 깊이에 도달한 경우
        if (state.isDone() || depth == 0) {
            WinningStatus status = state.getWinningStatus();
            if (status == WinningStatus::WIN) {
                return {-1, maximizing_player ? 1.0 : -1.0};
            } else if (status == WinningStatus::LOSE) {
                return {-1, maximizing_player ? -1.0 : 1.0};
            } else if (status == WinningStatus::DRAW) {
                return {-1, 0.0};
            }
            
            // 휴리스틱 평가 - 중앙 컬럼에 가중치 부여
            double score = 0.0;
            for (int col = 0; col < GameConstants::ConnectFour::W; col++) {
                double weight = 1.0;
                // 중앙에 가까울수록 가중치 증가
                weight = 1.0 + 0.3 * (3 - abs(col - 3));
                if (state.isValidMove(col)) {
                    score += weight * 0.1;
                }
            }
            
            // 플레이어에 따라 부호 변경
            return {-1, maximizing_player ? score : -score};
        }
        
        std::vector<int> legal_actions = state.legalActions();
        if (legal_actions.empty()) {
            return {-1, 0.0};
        }
        
        int best_action = legal_actions[0];
        double best_value = maximizing_player ? -std::numeric_limits<double>::infinity() : std::numeric_limits<double>::infinity();
        
        for (int action : legal_actions) {
            ConnectFourBitBoardState next_state = state;
            next_state.advance(action);
            
            double value = alphabeta(next_state, depth - 1, alpha, beta, !maximizing_player).second;
            
            if (maximizing_player) {
                if (value > best_value) {
                    best_value = value;
                    best_action = action;
                }
                alpha = std::max(alpha, best_value);
            } else {
                if (value < best_value) {
                    best_value = value;
                    best_action = action;
                }
                beta = std::min(beta, best_value);
            }
            
            // 알파-베타 가지치기
            if (beta <= alpha) {
                break;
            }
        }
        
        return {best_action, best_value};
    }
    
    // Iterative Deepening 구현 (비트보드 버전)
    int iterativeDeepening(const ConnectFourState& state) {
        // 일반 게임 상태를 비트보드 상태로 변환
        ConnectFourBitBoardState bit_state(state);
        
        int best_action = -1;
        
        // 가능한 모든 행동 확인
        std::vector<int> legal_actions = bit_state.legalActions();
        if (legal_actions.empty()) {
            return -1;
        }
        
        // 적어도 하나의 유효한 수는 반환
        best_action = legal_actions[random_engine_() % legal_actions.size()];
        
        auto start_time = std::chrono::high_resolution_clock::now();
        int max_depth = params_.searchDepth;
        
        // 시간 제한이 0이면 설정된 깊이까지만 탐색
        if (params_.timeThreshold <= 0) {
            return alphabeta(bit_state, max_depth, -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), true).first;
        }
        
        // 시간 제한 내에서 점진적으로 깊이 증가
        for (int depth = 1; depth <= max_depth; depth++) {
            int current_action = alphabeta(bit_state, depth, -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), true).first;
            
            // 유효한 행동을 찾았으면 업데이트
            if (current_action != -1) {
                best_action = current_action;
            }
            
            // 시간 확인
            auto current_time = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
            
            // 시간 제한에 근접했으면 현재까지의 최선의 수 반환
            if (elapsed >= params_.timeThreshold * 0.8) {
                break;
            }
        }
        
        return best_action;
    }
    
public:
    ConnectFourBitIterativeDeepeningAlgorithm() : random_engine_(std::random_device{}()) {}
    
    int selectAction(const GameState& state) override {
        auto connectfour_state = static_cast<const ConnectFourState&>(state);
        return iterativeDeepening(connectfour_state);
    }
    
    std::string getName() const override {
        return "Bit Iterative Deepening (Connect Four)";
    }
    
    void setParams(const AlgorithmParams& params) override {
        params_ = params;
    }
    
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override {
        auto connectfour_state = static_cast<const ConnectFourState&>(state);
        auto next_state = std::make_unique<ConnectFourState>(connectfour_state);
        next_state->progress(action);
        return next_state;
    }
};

// ----- 게임 타입별 알고리즘 생성 함수들 -----

// 단일 플레이어 컨텍스트 없음 (AutoMaze) 생성
std::unique_ptr<Algorithm> createSinglePlayerWithoutContextAlgorithm(
    const std::string& algorithmName, const AlgorithmParams& params) {
    
    std::unique_ptr<Algorithm> algorithm;
    
    if (algorithmName == "AutoMazeRandom") {
        algorithm = std::make_unique<AutoMazeRandomAlgorithm>();
    } else if (algorithmName == "HillClimb") {
        algorithm = std::make_unique<HillClimbAlgorithm>();
    } else if (algorithmName == "SimulatedAnnealing") {
        algorithm = std::make_unique<SimulatedAnnealingAlgorithm>();
    } else {
        throw std::invalid_argument("Unknown single player without context algorithm: " + algorithmName);
    }
    
    algorithm->setParams(params);
    return algorithm;
}

// 단일 플레이어 컨텍스트 있음 (Maze) 생성
std::unique_ptr<Algorithm> createSinglePlayerWithContextAlgorithm(
    const std::string& algorithmName, const AlgorithmParams& params) {
    
    std::unique_ptr<Algorithm> algorithm;
    
    if (algorithmName == "MazeRandom") {
        algorithm = std::make_unique<MazeRandomAlgorithm>();
    } else if (algorithmName == "Greedy") {
        algorithm = std::make_unique<GreedyAlgorithm>();
    } else if (algorithmName == "BeamSearch") {
        algorithm = std::make_unique<BeamSearchAlgorithm>();
    } else if (algorithmName == "Chokudai") {
        algorithm = std::make_unique<ChokudaiAlgorithm>();
    } else {
        throw std::invalid_argument("Unknown single player with context algorithm: " + algorithmName);
    }
    
    algorithm->setParams(params);
    return algorithm;
}

// 두 플레이어 교대 (TwoMaze) 생성
std::unique_ptr<Algorithm> createTwoPlayerAlternateAlgorithm(
    const std::string& algorithmName, const AlgorithmParams& params) {
    
    std::unique_ptr<Algorithm> algorithm;
    
    if (algorithmName == "TwoMazeRandom") {
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
    } else {
        throw std::invalid_argument("Unknown two player alternate algorithm: " + algorithmName);
    }
    
    algorithm->setParams(params);
    return algorithm;
}

// 두 플레이어 동시 (SimMaze) 생성
std::unique_ptr<Algorithm> createTwoPlayerSimultaneousAlgorithm(
    const std::string& algorithmName, const AlgorithmParams& params) {
    
    std::unique_ptr<Algorithm> algorithm;
    
    if (algorithmName == "SimMazeRandom") {
        algorithm = std::make_unique<SimMazeRandomAlgorithm>();
    } else if (algorithmName == "SimMazeDUCT") {
        algorithm = std::make_unique<DuctAlgorithm>();
    } else if (algorithmName == "SimMazePMC") {
        algorithm = std::make_unique<PMCAlgorithm>();
    } else if (algorithmName == "SimMazeMCTS") {
        algorithm = std::make_unique<MCTSSimAlgorithm>();
    } else {
        throw std::invalid_argument("Unknown two player simultaneous algorithm: " + algorithmName);
    }
    
    algorithm->setParams(params);
    return algorithm;
}

// Connect Four 알고리즘 생성
std::unique_ptr<Algorithm> createConnectFourAlgorithm(
    const std::string& algorithmName, const AlgorithmParams& params) {
    
    std::unique_ptr<Algorithm> algorithm;
    
    if (algorithmName == "ConnectFourRandom") {
        algorithm = std::make_unique<ConnectFourRandomAlgorithm>();
    } else if (algorithmName == "ConnectFourMCTS") {
        algorithm = std::make_unique<ConnectFourMCTSAlgorithm>();
    } else if (algorithmName == "ConnectFourBitMCTS") {
        algorithm = std::make_unique<ConnectFourBitMCTSAlgorithm>();
    } else if (algorithmName == "ConnectFourID") {
        algorithm = std::make_unique<ConnectFourIterativeDeepeningAlgorithm>();
    } else if (algorithmName == "ConnectFourBitID") {
        algorithm = std::make_unique<ConnectFourBitIterativeDeepeningAlgorithm>();
    } else {
        throw std::invalid_argument("Unknown Connect Four algorithm: " + algorithmName);
    }
    
    algorithm->setParams(params);
    return algorithm;
}

// 알고리즘 팩토리 메인 구현
std::unique_ptr<Algorithm> AlgorithmFactory::createAlgorithm(
    const std::string& algorithmName, const AlgorithmParams& params) {
    
    // 각 게임 타입에 맞는 알고리즘 이름 접두사를 검사
    
    // 단일 플레이어 컨텍스트 없음 (AutoMaze)
    if (algorithmName == "AutoMazeRandom" || 
        algorithmName == "HillClimb" || 
        algorithmName == "SimulatedAnnealing") {
        return createSinglePlayerWithoutContextAlgorithm(algorithmName, params);
    }
    
    // 단일 플레이어 컨텍스트 있음 (Maze)
    else if (algorithmName == "MazeRandom" || 
             algorithmName == "Greedy" || 
             algorithmName == "BeamSearch" || 
             algorithmName == "Chokudai") {
        return createSinglePlayerWithContextAlgorithm(algorithmName, params);
    }
    
    // 두 플레이어 교대 (TwoMaze)
    else if (algorithmName == "TwoMazeRandom" || 
             algorithmName == "Minimax" || 
             algorithmName == "AlphaBeta" || 
             algorithmName == "IterativeDeepening" || 
             algorithmName == "MonteCarlo" || 
             algorithmName == "MCTS" || 
             algorithmName == "Thunder" ||
             algorithmName == "ThunderTime") {
        return createTwoPlayerAlternateAlgorithm(algorithmName, params);
    }
    
    // 두 플레이어 동시 (SimMaze)
    else if (algorithmName == "SimMazeRandom" || 
             algorithmName == "SimMazeDUCT" || 
             algorithmName == "SimMazePMC" || 
             algorithmName == "SimMazeMCTS") {
        return createTwoPlayerSimultaneousAlgorithm(algorithmName, params);
    }
    
    // Connect Four
    else if (algorithmName == "ConnectFourRandom" || 
             algorithmName == "ConnectFourMCTS" || 
             algorithmName == "ConnectFourBitMCTS" ||
             algorithmName == "ConnectFourID" ||
             algorithmName == "ConnectFourBitID") {
        return createConnectFourAlgorithm(algorithmName, params);
    }
    
    else {
        throw std::invalid_argument("Unknown algorithm: " + algorithmName);
    }
}
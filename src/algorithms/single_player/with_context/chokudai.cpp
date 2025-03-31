#include "chokudai.h"
#include <queue>
#include <chrono>

int chokudaiSearchAction(const MazeState& state, const ChokudaiConfig& config) {
    auto beam = std::vector<std::priority_queue<MazeState>>(config.search_depth + 1);
    beam[0].push(state);

    TimeKeeper* time_keeper = nullptr;
    if (config.time_threshold > 0) {
        time_keeper = new TimeKeeper(config.time_threshold);
    }
    
    for (int cnt = 0; cnt < config.search_number; cnt++) {
        for (int t = 0; t < config.search_depth; t++) {
            auto& now_beam = beam[t];
            auto& next_beam = beam[t + 1];
            
            for (int i = 0; i < config.search_width; i++) {

                if (time_keeper && time_keeper->isTimeOver()) 
                {
                    break;
                }

                if (now_beam.empty()) break;
                MazeState now_state = now_beam.top();
                if (now_state.isDone()) break;
                now_beam.pop();
                
                for (const auto& action : now_state.legalActions()) {
                    MazeState next_state = now_state;
                    next_state.progress(action);
                    next_state.evaluateScore();
                    if (t == 0) next_state.first_action_ = action;
                    next_beam.push(next_state);
                }
            }

            if (time_keeper && time_keeper->isTimeOver()) 
            {
                break;
            }
        }

        if (time_keeper && time_keeper->isTimeOver()) 
        {
            break;
        }
    }

    if (time_keeper) 
    {
        delete time_keeper;
    }
    
    for (int t = config.search_depth; t >= 0; t--) {
        if (!beam[t].empty()) {
            return beam[t].top().first_action_;
        }
    }
    return -1;
} 
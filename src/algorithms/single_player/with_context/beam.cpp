#include "beam.h"
#include <queue>
#include <iostream>
#include <random>

int beamSearchAction(const MazeState& state, const BeamConfig& config)
{
    std::priority_queue<MazeState> now_beam;
    MazeState best_state;

    TimeKeeper* time_keeper = nullptr;
    if (config.time_threshold > 0) {
        time_keeper = new TimeKeeper(config.time_threshold);
    }

    now_beam.push(state);
    for (int t = 0; t < config.search_depth; t++)
    {
        std::priority_queue<MazeState> next_beam;
        for (int i = 0; i < config.search_width; i++)
        {
            if (time_keeper && time_keeper->isTimeOver()) {
                break;
            }
            
            if (now_beam.empty())
                break;
            
            MazeState now_state = now_beam.top();
            now_beam.pop();   
             
            for (const auto& action : now_state.legalActions())
            {
                MazeState next_state = now_state;
                next_state.progress(action);
                next_state.evaluateScore();
                if (t == 0)
                {
                    next_state.first_action_ = action;
                }
                next_beam.push(next_state);
            }
        }
        
        if (time_keeper && time_keeper->isTimeOver()) {
            break;
        }
        
        now_beam = next_beam;
        if (!now_beam.empty()) {
            best_state = now_beam.top();
        }

        if (best_state.isDone())
        {
            break;
        }
    }
    
    if (time_keeper) {
        delete time_keeper;
    }
    
    return best_state.first_action_;
}
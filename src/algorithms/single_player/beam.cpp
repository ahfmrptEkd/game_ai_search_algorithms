#include "beam.h"
#include <queue>
#include <iostream>
#include <random>

int beamSearchAction(const State& state, const BeamConfig& config)
{
    auto legal_actions = state.legalActions();
    std::priority_queue<State> now_beam;
    State best_state;

    now_beam.push(state);
    for (int t = 0; t < config.beam_depth; t++)
    {
        std::priority_queue<State> next_beam;
        for (int i = 0; i < config.beam_width; i++)
        {
            if (now_beam.empty())
                break;
            
            State now_state = now_beam.top();
            now_beam.pop();
            for (const auto& action : legal_actions)
            {
                State next_state = now_state;
                next_state.progress(action);
                next_state.evaluateScore();
                if (t == 0)
                {
                    next_state.first_action_ = action;
                }
                next_beam.push(next_state);
            }
        }
        
        now_beam = next_beam;
        best_state = now_beam.top();

        if (best_state.isDone())
        {
            break;
        }
    }
    return best_state.first_action_;
}
#include "chokudai.h"
#include <queue>
#include <chrono>

int chokudaiSearchAction(const State& state, const ChokudaiConfig& config) {
    auto beam = std::vector<std::priority_queue<State>>(config.beam_depth + 1);
    beam[0].push(state);
    
    for (int cnt = 0; cnt < config.beam_number; cnt++) {
        for (int t = 0; t < config.beam_depth; t++) {
            auto& now_beam = beam[t];
            auto& next_beam = beam[t + 1];
            
            for (int i = 0; i < config.beam_width; i++) {
                if (now_beam.empty()) break;
                State now_state = now_beam.top();
                if (now_state.isDone()) break;
                now_beam.pop();
                
                for (const auto& action : now_state.legalActions()) {
                    State next_state = now_state;
                    next_state.progress(action);
                    next_state.evaluateScore();
                    if (t == 0) next_state.first_action_ = action;
                    next_beam.push(next_state);
                }
            }
        }
    }
    
    for (int t = config.beam_depth; t >= 0; t--) {
        if (!beam[t].empty()) {
            return beam[t].top().first_action_;
        }
    }
    return -1;
} 
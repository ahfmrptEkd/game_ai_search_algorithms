// src/examples/twomaze_demo.cpp
#include "../algorithms/two_player/alternate/random.h"
#include "../algorithms/two_player/alternate/minimax.h"
#include "../algorithms/two_player/alternate/alphabeta.h"
#include "../algorithms/two_player/alternate/deepening.h"
#include "../algorithms/two_player/alternate/mc.h"
#include "../algorithms/two_player/alternate/mcts.h"
#include "../algorithms/two_player/alternate/thunder.h"
#include "../common/game_util.h"
#include <iostream>
#include <string>
#include <map>
#include <functional>

int main(int argc, char* argv[]) 
{
    GameUtil::mt_for_action.seed(0);
    
    // 사용 가능한 알고리즘을 맵으로 관리 (이름 -> 함수 포인터)
    std::map<std::string, std::function<void(int)>> algorithms = 
    {
        {"random", [](int seed) { playGameRandom(seed); }},
        {"minimax", [](int seed) { playGameMinimax(seed); }},
        {"alphabeta", [](int seed) {playGameAlphaBeta(seed);}},
        {"deepening", [](int seed) {playGameIterativeDeepening(seed);}},
        {"mc", [](int seed) {playGameMonteCarlo(seed);}},
        {"mcts", [](int seed) {playGameMCTS(seed);}},
        {"thunder", [](int seed) {playGameThunder(seed, 1000);}},
        {"thunder_time", [](int seed) {playGameThunderWithTime(seed, 100);}}
        // 다른 알고리즘 추가
    };
    
    std::string algorithm = "random";
    
    if (argc > 1) {
        algorithm = argv[1];
    }
    
    auto it = algorithms.find(algorithm);
    if (it != algorithms.end()) 
    {
        std::cout << "Running " << algorithm << " algorithm...\n";
        it->second(GameUtil::mt_for_action());
    } 
    else 
    {
        std::cout << "Unknown algorithm: " << algorithm << "\n";
        std::cout << "Available algorithms:";
        for (const auto& pair : algorithms) {
            std::cout << " " << pair.first;
        }
        std::cout << "\n";
        return 1;
    }
    
    return 0;
}
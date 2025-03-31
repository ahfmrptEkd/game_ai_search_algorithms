// src/examples/twomaze_demo.cpp
#include "../algorithms/two_player/alternate/random.h"
#include "../common/game_util.h"
#include <iostream>
#include <string>
#include <map>
#include <functional>

int main(int argc, char* argv[]) 
{
    GameUtil::mt_for_action.seed(0);
    
    std::map<std::string, std::function<void(int)>> algorithms = 
    {
        {"random", playGameTwoMazeRandom},
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
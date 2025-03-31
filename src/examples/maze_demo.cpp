#include "../algorithms/single_player/with_context/random.h"
#include "../algorithms/single_player/with_context/greedy.h"
#include "../algorithms/single_player/with_context/beam.h"
#include "../algorithms/single_player/with_context/chokudai.h"
#include "../common/coord.h"
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
        {"random", playGameRandom},
        {"greedy", playGameGreedy},
        {"beam", [](int seed){ playGameBeam(seed, 1); }},        
        {"chokudai", [](int seed){ playGameChokudai(seed, 1); }} 
    };
    
    std::string algorithm = "random";
    
    if (argc > 1) {
        algorithm = argv[1];
    }
    
    // 알고리즘 존재 여부 확인 후 실행
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
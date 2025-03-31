#include "../algorithms/single_player/without_context/random.h"
#include <iostream>
#include <string>
#include <map>
#include <functional>

int main(int argc, char* argv[]) 
{
    const int seed = 12345;
    
    std::map<std::string, std::function<void(int)>> algorithms = 
    {
        {"random", playGameRandom}
        // 나중에 다른 알고리즘 추가 가능
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
        it->second(seed);
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
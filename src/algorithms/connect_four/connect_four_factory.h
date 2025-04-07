#ifndef CONNECT_FOUR_FACTORY_H
#define CONNECT_FOUR_FACTORY_H

#include <memory>
#include <string>
#include "../algorithm_interface.h"

// Connect Four 게임을 위한 팩토리 클래스
class ConnectFourFactory {
public:
    // 알고리즘 생성 메소드
    static std::unique_ptr<Algorithm> createAlgorithm(
        const std::string& algorithmName, const AlgorithmParams& params = AlgorithmParams());
};

#endif // CONNECT_FOUR_FACTORY_H 
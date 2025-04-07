#ifndef TWO_PLAYER_SIMULTANEOUS_FACTORY_H
#define TWO_PLAYER_SIMULTANEOUS_FACTORY_H

#include <memory>
#include <string>
#include "../../algorithm_interface.h"

// 두 플레이어 동시 게임을 위한 팩토리 클래스
class TwoPlayerSimultaneousFactory {
public:
    // 알고리즘 생성 메소드
    static std::unique_ptr<Algorithm> createAlgorithm(
        const std::string& algorithmName, const AlgorithmParams& params = AlgorithmParams());
};

#endif // TWO_PLAYER_SIMULTANEOUS_FACTORY_H 
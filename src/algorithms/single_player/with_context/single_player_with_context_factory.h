#ifndef SINGLE_PLAYER_WITH_CONTEXT_FACTORY_H
#define SINGLE_PLAYER_WITH_CONTEXT_FACTORY_H

#include <memory>
#include <string>
#include "../../algorithm_interface.h"

// 단일 플레이어 컨텍스트 있는 알고리즘을 위한 팩토리 클래스
class SinglePlayerWithContextFactory {
public:
    // 알고리즘 생성 메소드
    static std::unique_ptr<Algorithm> createAlgorithm(
        const std::string& algorithmName, const AlgorithmParams& params = AlgorithmParams());
};

#endif // SINGLE_PLAYER_WITH_CONTEXT_FACTORY_H 
#ifndef ALGORITHM_INTERFACE_H
#define ALGORITHM_INTERFACE_H

#include <memory>
#include "../common/game_state.h"

// 알고리즘 파라미터 구조체
struct AlgorithmParams {
    int searchWidth = GameConstants::AlgorithmParams::SEARCH_WIDTH;
    int searchDepth = GameConstants::AlgorithmParams::SEARCH_DEPTH;
    int searchNumber = GameConstants::AlgorithmParams::SEARCH_NUMBER;
    int64_t timeThreshold = GameConstants::AlgorithmParams::TIME_THRESHOLD;
    
    // 추가 필요한 파라미터들
    int playoutNumber = GameConstants::TwoMaze::PLAYOUT_NUMBER;  // monte carlo
    int startTemperature = GameConstants::AlgorithmParams::START_TEMPERATURE;
    int endTemperature = GameConstants::AlgorithmParams::END_TEMPERATURE;

    // 동시 게임용 파라미터
    int playerId = 0;
};

// 알고리즘 인터페이스
class Algorithm {
public:
    virtual ~Algorithm() = default;
    
    // 알고리즘 실행 메소드
    virtual int selectAction(const GameState& state) = 0;
    
    // 알고리즘 이름 반환
    virtual std::string getName() const = 0;
    
    // 파라미터 설정
    virtual void setParams(const AlgorithmParams& params) = 0;
    
    // 상태 실행 및 평가
    virtual std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) = 0;
};

// 알고리즘 팩토리 클래스
class AlgorithmFactory {
public:
    // 알고리즘 생성 메소드
    static std::unique_ptr<Algorithm> createAlgorithm(const std::string& algorithmName, 
                                                     const AlgorithmParams& params = AlgorithmParams());
};

#endif // ALGORITHM_INTERFACE_H 
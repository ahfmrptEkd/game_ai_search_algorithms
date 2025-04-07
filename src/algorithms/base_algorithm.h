#ifndef BASE_ALGORITHM_H
#define BASE_ALGORITHM_H

#include "algorithm_interface.h"
#include <string>

// 모든 알고리즘의 기본 클래스
// 공통 기능을 구현하여 코드 중복을 줄임
class BaseAlgorithm : public Algorithm {
protected:
    AlgorithmParams params_;
    std::string name_;
    
public:
    BaseAlgorithm(const std::string& name) : name_(name) {}
    virtual ~BaseAlgorithm() = default;
    
    // 알고리즘 이름 반환 (공통 구현)
    std::string getName() const override {
        return name_;
    }
    
    // 파라미터 설정 (공통 구현)
    void setParams(const AlgorithmParams& params) override {
        params_ = params;
    }
    
    // 상태 실행 및 평가 (각 게임 타입별로 다르게 구현되어야 함)
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override = 0;
    
    // 액션 선택 (알고리즘별로 구현되어야 함)
    int selectAction(const GameState& state) override = 0;
};

#endif // BASE_ALGORITHM_H 
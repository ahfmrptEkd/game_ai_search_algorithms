#include "game_util.h"

// 난수 생성기 초기화
namespace GameUtil {
    std::mt19937 mt_for_action(0);
} 

void printGameResult(WinningStatus status) {
    switch (status) {
        case WinningStatus::WIN:
            std::cout << "플레이어 A가 승리했습니다!" << std::endl;
            break;
        case WinningStatus::LOSE:
            std::cout << "플레이어 B가 승리했습니다!" << std::endl;
            break;
        case WinningStatus::DRAW:
            std::cout << "무승부입니다!" << std::endl;
            break;
        default:
            std::cout << "게임이 정상적으로 종료되지 않았습니다." << std::endl;
            break;
    }
} 
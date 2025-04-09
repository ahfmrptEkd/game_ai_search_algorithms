#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

#include <cstdint>

// 모든 게임에서 공통으로 사용하는 상수
namespace GameConstants {
    using ScoreType = int64_t;
    constexpr const ScoreType INF = 1000000000LL;
    
    constexpr const int DX[4] = {1, -1, 0, 0};
    constexpr const int DY[4] = {0, 0, 1, -1};
    
    // 모든 게임에 공통으로 사용되는 보드 관련 상수
    namespace Board {
        constexpr const int H = 8;
        constexpr const int W = 8;
        constexpr const int END_TURN = 15;
    }
    
    namespace AutoMaze {
        // AutoMaze에만 있는 특수 상수
        constexpr const int CHARACTER_N = 3;
    }

    namespace TwoMaze {
        constexpr const int PLAYER_N = 2;
        constexpr const int PLAYOUT_NUMBER = 1000;
    }
    
    // 알고리즘 파라미터 상수 추가
    namespace AlgorithmParams {
        constexpr const int SEARCH_WIDTH = 3;
        constexpr const int SEARCH_DEPTH = 7;
        constexpr const int SEARCH_NUMBER = 4;
        constexpr const int64_t TIME_THRESHOLD = 10;
        constexpr const double START_TEMPERATURE = 500.0;
        constexpr const double END_TEMPERATURE = 10.0;
    }

    namespace ConnectFour {
        constexpr const int H = 6;
        constexpr const int W = 7;
        constexpr const int CONNECT_NUMBER = 4;
        constexpr const int PLAYOUT_NUMBER = 1000;
    }
}

#endif // GAME_CONSTANTS_H 
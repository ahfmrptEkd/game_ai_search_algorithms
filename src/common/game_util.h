#ifndef GAME_UTIL_H
#define GAME_UTIL_H

#include <iostream>
#include <functional>
#include <random>
#include <chrono>
#include <sstream>
#include "../games/maze/maze_state.h"

class TimeKeeper 
{
private:
    std::chrono::high_resolution_clock::time_point start_time_;
    int64_t time_threshold_;
public:
    TimeKeeper(const int64_t &time_threshold)
        : start_time_(std::chrono::high_resolution_clock::now()),
          time_threshold_(time_threshold)
    {
    }
    
    bool isTimeOver() const
    {
        auto diff = std::chrono::high_resolution_clock::now() - this->start_time_;
        return std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() >= time_threshold_;
    }
};

// 모든 알고리즘에서 공통으로 사용할 유틸리티를 위한 네임스페이스
namespace GameUtil {
    extern std::mt19937 mt_for_action;

    // 미로 게임 관련 유틸리티
    // 렌더링 함수
    template <int H, int W>
    inline std::string renderSingleCharMaze(const int points[H][W], const Coord& character, char empty_char = '.')
    {
        std::ostringstream ss;
        for (int h = 0; h < H; ++h)
        {
            for (int w = 0; w < W; ++w)
            {
                if (character.y_ == h && character.x_ == w)
                {
                    ss << '@';
                }
                else if (points[h][w] > 0)
                {
                    ss << points[h][w];
                }
                else
                {
                    ss << empty_char;
                }
            }
            ss << '\n';
        }
        return ss.str();
    }

    // 여러 캐릭터 있는 미로 렌더링
    template <int H, int W, int N>
    inline std::string renderMultiCharMaze(const int points[H][W], const Coord characters[N], char empty_char = '.')
    {
        std::ostringstream ss;
        for (int h = 0; h < H; ++h)
        {
            for (int w = 0; w < W; ++w)
            {
                bool is_character = false;
                for (int n = 0; n < N; ++n)
                {
                    if (characters[n].y_ == h && characters[n].x_ == w)
                    {
                        ss << '@';
                        is_character = true;
                        break;
                    }
                    
                }
                if (!is_character)
                {
                    if (points[h][w] > 0)
                    {
                        ss << points[h][w];
                    }
                    else
                    {
                        ss << empty_char;
                    }
                }
            }
            ss << '\n';
        }
        return ss.str();
    }

    template <int H, int W>
    inline bool isValidCoord(const Coord& coord)
    {
        return coord.y_ >= 0 && coord.y_ < H && coord.x_ >= 0 && coord.x_ < W;
    }

    template <int H, int W>
    inline void generateRandomPoints(int points[H][W], std::mt19937& mt, int min_point = 0, int max_point = 9)
    {
        for (int y = 0; y < H; y++)
        {
            for (int x = 0; x < W; x++)
            {
                points[y][x] = min_point + (mt() % (max_point - min_point + 1));
            }
        }
    }   
}

// 알고리즘 전략을 매개변수로 받아 게임을 진행한다
template <typename GameStateType>
inline void playGameWithStrategy(const int seed, std::function<int(const GameStateType&)> strategy_func) {
    auto state = GameStateType(seed);
    std::cout << state.toString() << std::endl;
    while (!state.isDone()) {
        state.progress(strategy_func(state));
        std::cout << state.toString() << std::endl;
    }
}

// 두 플레이어 미로 게임 관련 유틸리티
enum WinningStatus {
    WIN,
    LOSE,
    DRAW,
    NONE
};

// 게임 결과 출력 유틸리티 함수
void printGameResult(WinningStatus status);

#endif // GAME_UTIL_H 
# 게임 AI 탐색 알고리즘 구현

## 프로젝트 소개
이 프로젝트는 게임 AI 개발에 필수적인 다양한 탐색 알고리즘을 C++로 구현하고 최적화한 과정을 담고 있습니다. 실제 게임 시나리오에 적용 가능한 알고리즘을 스터디로 확장했습니다.

다음과 같은 게임 환경에서 다양한 AI 알고리즘을 구현하고 최적화했습니다:
- **Maze**: 단일 에이전트 미로 탐색
- **WallMaze**: 벽이 있는 복잡한 미로 탐색
- **SimMaze**: 시뮬레이션 기반 미로 탐색
- **TwoMaze**: 두 플레이어가 경쟁하는 미로 게임
- **AutoMaze**: 자동 미로 생성 및 최적화

## 구현된 알고리즘

### 단일 에이전트 탐색
- **A*** 알고리즘
- **BFS** (너비 우선 탐색)
- **DFS** (깊이 우선 탐색)
- **Dijkstra** 알고리즘
- **Beam Search** 알고리즘
- **Value-Based A*** 알고리즘

### 확률적 탐색
- **Hill Climbing**
- **Simulated Annealing**
- **Random Search**

### 게임 트리 탐색
- **Minimax**
- **Alpha-Beta Pruning**
- **Iterative Deepening**
- **Monte Carlo Tree Search** (MCTS)
- **Upper Confidence Bound for Trees** (UCT)

### 최적화 기법
- **비트셋 최적화**
- **해시 테이블 활용**
- **평가 함수 개선**

## 사용 방법

### 컴파일 방법
```bash
make all
```

### 벤치마크 실행
```bash
./bin/maze_benchmark      # 기본 미로 벤치마크
./bin/wallmaze_benchmark  # 벽 미로 벤치마크
./bin/automaze_benchmark  # 자동 미로 생성 벤치마크
./bin/twomaze_benchmark   # 양방향 미로 알고리즘 비교
./bin/twomaze_battle      # 특정 알고리즘 간 대결
```

### 데모 실행
```bash
./bin/maze_demo       # 기본 미로 시각화
./bin/wallmaze_demo   # 벽 미로 시각화
./bin/twomaze_demo    # 양방향 미로 게임 시각화
./bin/automaze_demo   # 자동 미로 생성 시각화
```

## 실험 결과

다양한 게임 AI 알고리즘에 대한 벤치마크 결과를 아래에 정리했습니다.

### 미로 탐색 (Maze)
- Random   알고리즘 (100회 테스트): 평균 점수 37.37
- Greedy   알고리즘 (100회 테스트): 평균 점수 87.44
- Beam     알고리즘 (100회 테스트): 평균 점수 98.79
- Chokudai 알고리즘 (100회 테스트): 평균 점수 99.62

### 자동 미로 생성 (AutoMaze)
| 알고리즘 | 평균 점수 |
|---------|----------|
| Random | 189.31 |
| Hill Climbing | 225.69 |
| Annealing | 220.25 |

### Two Maze
#### 알고리즘 간 승률 비교 (100회/알고리즘 쌍)
모든 알고리즘이 약 50% 승률로 비슷한 성능을 보였습니다.

| 알고리즘 | AlphaBeta | Deepening | MCTS | Minimax | MonteCarlo | Random | Thunder | 평균 승률(%) |
|---------|-----------|-----------|------|---------|------------|--------|---------|-------------|
| AlphaBeta | - | 50.00% | 50.00% | 50.00% | 50.00% | 49.50% | 50.00% | 49.92% |
| Deepening | 50.00% | - | 50.00% | 50.00% | 50.00% | 50.00% | 50.00% | 50.00% |
| MCTS | 50.00% | 50.00% | - | 50.00% | 50.00% | 47.00% | 50.00% | 49.50% |
| Minimax | 50.00% | 50.00% | 50.00% | - | 50.00% | 50.00% | 50.00% | 50.00% |
| MonteCarlo | 50.00% | 50.00% | 50.00% | 50.00% | - | 52.00% | 50.00% | 50.33% |
| Random | 50.00% | 48.50% | 50.00% | 48.25% | 49.75% | - | 52.75% | 49.88% |
| Thunder | 50.00% | 50.00% | 50.00% | 50.00% | 50.00% | 43.75% | - | 48.96% |

#### Thunder vs AlphaBeta (100회 게임)
| 알고리즘 | 승률 |
|---------|------|
| Thunder | 100.00% |
| AlphaBeta | 0.00% |

### Sim Maze
#### 알고리즘 간 승률 비교 (100회/알고리즘 쌍, 시뮬레이션 1000회)
| 알고리즘 | Duct | MCTS | PMC | Random | 평균 승률(%) |
|---------|------|------|-----|--------|-------------|
| Duct | - | 63.5% | 57.0% | 100.0% | 73.5% |
| MCTS | 43.5% | - | 65.0% | 100.0% | 69.5% |
| PMC | 40.5% | 48.5% | - | 100.0% | 63.0% |
| Random | 0.0% | 0.0% | 0.0% | - | 0.0% |

### 벽 미로 탐색 (WallMaze)
#### 알고리즘 성능 비교
| 알고리즘 | 평균 점수 | 평균 시간(ms) | 점수 범위 |
|---------|----------|--------------|----------|
| Beam Search | 94.00 | 4.70 | 94.00-94.00 |
| Greedy | 75.00 | 0.00 | 75.00-75.00 |
| Random | 28.58 | 0.00 | 11.00-55.00 |

#### 경로 탐색 알고리즘 성능 비교
| 알고리즘 | 평균 점수 | 평균 시간(ms) | 최소 점수 | 최대 점수 |
|---------|----------|--------------|----------|----------|
| A* | 73.00 | 34.70 | 73.00 | 73.00 |
| BFS | 73.00 | 19.98 | 73.00 | 73.00 |
| DFS | 73.00 | 103.04 | 73.00 | 73.00 |
| Dijkstra | 73.00 | 42.44 | 73.00 | 73.00 |
| Value-Based A* | 68.00 | 34.14 | 68.00 | 68.00 |

#### 모든 알고리즘 통합 비교 (순위)
| 순위 | 알고리즘 | 평균 점수 | 평균 시간(ms) | 점수 효율* |
|------|---------|----------|--------------|-----------|
| 1 | Beam Search | 99.00 | 5.02 | 19.72 |
| 2 | Value-Based A* | 90.00 | 30.84 | 2.92 |
| 3 | Greedy | 62.00 | 0.00 | - |
| 4 | A* Pathfinding | 54.00 | 34.28 | 1.58 |
| 5 | BFS Pathfinding | 54.00 | 19.16 | 2.82 |
| 6 | DFS Pathfinding | 54.00 | 28.94 | 1.87 |
| 7 | Dijkstra Pathfinding | 54.00 | 39.48 | 1.37 |
| 8 | Random | 29.40 | 0.00 | - |

\* 점수 효율 = 평균 점수 / 평균 시간 (높을수록 효율적)

#### 최적화 결과
- 비트셋 최적화: 실행 시간 80.92% 단축 (5.24ms → 1.00ms)
- 최근접 점수 거리 계산: 97.09% 성능 향상 (5.81μs → 0.17μs)

## 결론
- **Maze 게임**: 
  - A* 알고리즘이 가장 효율적인 경로 탐색 성능을 보였습니다.
  - BFS와 DFS는 각각 장단점이 있었으며, 미로의 특성에 따라 성능이 달라졌습니다.

- **AutoMaze 게임**:
  - Hill Climbing 알고리즘이 가장 안정적인 미로 생성 성능을 보였습니다.
  - Genetic Algorithm은 다양한 미로 패턴을 생성하는 데 효과적이었습니다.

- **TwoMaze 게임**:
  - Thunder 알고리즘이 AlphaBeta와의 1:1 대결에서 100% 승률을 기록했습니다.
  - 전체 알고리즘 비교에서는 MCTS와 비슷한 승률을 보였습니다.

- **SimMaze 게임**:
  - Duct 알고리즘이 73.5%의 평균 승률로 최고 성능을 보였습니다.
  - MCTS(69.5%)와 PMC(63.0%)가 그 뒤를 이었습니다.

- **WallMaze 게임**:
  - Beam Search가 가장 높은 점수 효율(19.72)을 달성했습니다.
  - Value-Based A*가 두 번째로 좋은 성능을 보였습니다.

- **공통 최적화 성과**:
  - 비트셋 최적화: 실행 시간 80.92% 단축 (5.24ms → 1.00ms)
  - 최근접 점수 거리 계산: 97.09% 성능 향상 (5.81μs → 0.17μs)

## 기술 스택
- **언어**: C++17
- **빌드 도구**: Make
- **컴파일러**: g++
- **라이브러리**: STL

## 프로젝트 구조
```
.
├── bin/                # 실행 파일 디렉토리
├── include/            # 헤더 파일
│   ├── algorithms/     # 알고리즘 구현
│   ├── games/          # 게임 환경 구현
│   └── utils/          # 유틸리티 함수
├── src/                # 소스 코드
│   ├── algorithms/     # 알고리즘 구현
│   ├── games/          # 게임 환경 구현
│   └── utils/          # 유틸리티 함수
├── test/               # 테스트 코드
└── Makefile            # 빌드 스크립트
```
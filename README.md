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

## 주요 벤치마크 결과

다음은 핵심 벤치마크 결과의 하이라이트입니다:

- **Beam Search**: WallMaze에서 99.00점, 실행시간 5.02ms로 최고 성능 (점수 효율 19.72)
- **Duct 알고리즘**: SimMaze에서 73.5% 승률로 최고 성능 달성
- **Thunder 알고리즘**: AlphaBeta와의 1:1 대결에서 100% 승률 기록
- **비트셋 최적화**: 실행 시간 80.92% 단축 (5.24ms → 1.00ms)

[모든 벤치마크 결과 보기](results/benchmark.md)

## 주요 결론

- **경로 탐색**: A* 알고리즘이 대부분의 시나리오에서 최적의 경로 탐색 성능을 보였으며, 특히 휴리스틱 함수의 선택이 성능에 큰 영향을 미쳤습니다.

- **게임 트리 탐색**: Thunder 알고리즘이 전통적인 알고리즘(AlphaBeta)보다 우수한 성능을 보였으며, 시간 제약 환경에서는 MCTS가 안정적인 성능을 유지했습니다.

- **최적화 기법**: 비트셋 최적화를 통해 거리 계산 시간을 97.09% 단축할 수 있었으며, 이는 실시간 게임 AI에 중요한 개선점입니다.

- **자동 생성**: Hill Climbing 알고리즘이 가장 안정적인 미로 생성 성능을 보였으며, Simulated Annealing은 지역 최적해 문제를 효과적으로 해결했습니다.

## 기술 스택
- **언어**: C++17
- **빌드 도구**: Make
- **컴파일러**: g++
- **라이브러리**: STL

## 프로젝트 구조
```
.
├── bin/                # 실행 파일 디렉토리
│   ├── maze_demo       # 기본 미로 시각화
│   ├── wallmaze_demo   # 벽 미로 시각화
│   ├── simmaze_demo    # 시뮬레이션 미로 시각화
│   ├── twomaze_demo    # 양방향 미로 게임 시각화
│   ├── automaze_demo   # 자동 미로 생성 시각화
│   ├── connect_four_demo # 커넥트 포 게임 시각화
│   ├── maze_benchmark  # 기본 미로 벤치마크
│   ├── wallmaze_benchmark # 벽 미로 벤치마크
│   ├── simmaze_benchmark # 시뮬레이션 미로 벤치마크
│   ├── twomaze_benchmark # 양방향 미로 알고리즘 비교
│   ├── automaze_benchmark # 자동 미로 생성 벤치마크
│   └── twomaze_battle  # 특정 알고리즘 간 대결
├── src/                # 소스 코드
│   ├── algorithms/     # 알고리즘 구현
│   │   ├── pathfinding/ # 경로 탐색 알고리즘
│   │   ├── single_player/ # 단일 플레이어 알고리즘
│   │   ├── two_player/ # 두 플레이어 알고리즘
│   │   └── connect_four/ # 커넥트 포 알고리즘
│   ├── games/          # 게임 환경 구현
│   │   ├── maze/       # 기본 미로 게임
│   │   ├── wallmaze/   # 벽 미로 게임
│   │   ├── simmaze/    # 시뮬레이션 미로 게임
│   │   ├── twomaze/    # 양방향 미로 게임
│   │   ├── automaze/   # 자동 미로 생성 게임
│   │   └── connect_four/ # 커넥트 포 게임
│   ├── common/         # 공통 유틸리티
│   └── examples/       # 예제 코드
├── results/            # 벤치마크 결과
├── Makefile            # 빌드 스크립트
└── test.sh             # 테스트 스크립트
```
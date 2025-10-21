# 🎮 C++ 게임 AI 탐색 알고리즘 구현 및 벤치마킹

> 다양한 고전 게임 AI 탐색 알고리즘(Minimax, MCTS, A*, Beam Search 등)을 C++로 구현하고, 여러 게임 환경에서 성능을 체계적으로 벤치마킹 및 분석한 프로젝트

![C++](https://img.shields.io/badge/C++-11+-blue.svg)
![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)
![License](https://img.shields.io/badge/license-MIT-blue.svg)

---

<br>

## 🎯 프로젝트 개요

* **S (Situation / 문제)**
  
  게임 AI 개발에는 다양한 탐색 알고리즘이 사용되지만, 각 알고리즘의 이론적 장단점과 실제 게임 환경에서의 성능 특성(시간 복잡도, 메모리 사용량, 해답의 질)을 **실증적으로 비교하고 이해**할 필요가 있었습니다. 특히 C++ 환경에서의 효율적인 구현과 최적화 기법 적용이 중요했습니다.
<br>

* **T (Task / 목표)**
  
  다양한 **고전 게임 AI 탐색 알고리즘** (경로 탐색, 싱글 플레이어, 2인 제로섬 게임)을 **C++로 직접 구현**하고, **여러 게임 환경**(다양한 미로 변형, 커넥트 포)에서 각 알고리즘의 **성능을 체계적으로 벤치마킹**하여 비교 분석하는 것이 목표였습니다.
<br>

* **A (Action / 해결)**

  1. **알고리즘 구현**: 경로 탐색(A*, BFS, DFS, Dijkstra), 싱글 플레이어(빔 탐색, 초쿠다이 탐색, 탐욕, 언덕 오르기, 담금질 기법), 2인 제로섬(Minimax, Alpha-Beta, MCTS, 반복 심화, Monte Carlo) 등 **15개 이상의 알고리즘**을 C++ 표준 라이브러리를 활용하여 구현했습니다. (`src/algorithms/`)

  2. **게임 환경 구축**: 알고리즘 테스트를 위해 **5가지 이상의 게임 환경**(기본 미로, 동시 이동 미로, 벽 설치 미로, 자동 생성 미로, 커넥트 포)을 `GameState` 추상 클래스를 기반으로 설계 및 구현했습니다. (`src/games/`)

  3. **최적화**: 커넥트 포 게임의 경우, **비트보드(Bitboard)** 표현(`connect_four_bitboard.h`)을 도입하여 상태 표현 및 연산 효율성을 극대화했습니다.

  4. **벤치마킹 시스템**: 각 게임 환경별로 데모(`*_demo`)와 벤치마크(`*_benchmark`) 실행 파일을 분리하고, `Makefile`을 통해 빌드 및 실행을 자동화했습니다. 벤치마크는 알고리즘별 탐색 시간, 점수 등을 측정합니다. (`src/examples/`, `docs/benchmark.md`)
<br>

* **R (Result / 결과)**
  
  * **핵심 산출물**: 다양한 게임 AI 알고리즘의 C++ 구현체 및 각 알고리즘의 성능을 비교 분석한 **벤치마크 결과** (`docs/benchmark.md`)를 도출했습니다. 데모 및 벤치마크 실행 파일(`bin/`)을 제공하여 구현된 알고리즘의 동작을 직접 확인하고 성능을 재현할 수 있습니다.
  * **기술적 성과**: 알고리즘별 성능 특성(예: MCTS의 강력함, Alpha-Beta의 효율성, 경로 탐색 알고리즘 간 시간/정확도 트레이드오프)을 **정량적 데이터**로 확인했습니다. **C++를 사용한 효율적인 알고리즘 구현** 능력과 **체계적인 성능 분석 및 비교** 역량을 입증했습니다.

---
<br>


## 🛠️ 기술적 구현 (Action): 핵심 요소

### 1. 다양한 알고리즘 스펙트럼 구현

이 프로젝트는 게임 AI의 핵심적인 탐색 문제들을 포괄하는 다양한 알고리즘을 다룹니다.

* **경로 탐색 (`algorithms/pathfinding/`)**:
  * **기본**: BFS, DFS
  * **최단 경로**: Dijkstra, A* (휴리스틱 사용)
  * **구현**: 각 알고리즘은 `Pathfinding` 추상 클래스를 상속받아 `findPath` 메서드를 구현합니다. `GameState`의 `get_legal_actions`, `get_next_state` 등을 활용합니다.

* **싱글 플레이어 게임 (`algorithms/single_player/`)**:
  * **Context-Aware (매 턴 최적수 계산)**: Greedy, Beam Search, Chokudai Beam Search, Random Action
  * **Context-Free (전체 해 탐색)**: Hill Climbing, Simulated Annealing
  * **특징**: 시간 제한 내 최고 점수 도달 목표. 평가 함수(`evaluateScore`) 중요.

* **2인 제로섬 게임 (경쟁)**:
  * **번갈아 두는 턴 (`algorithms/two_player/alternate/`)**: Minimax, Alpha-Beta Pruning, Iterative Deepening, Monte Carlo (MC), Monte Carlo Tree Search (MCTS), Thunder (MCTS 변형)
  * **동시 이동 (`algorithms/two_player/simultaneous/`)**: Playout Monte Carlo (PMC), Simultaneous MCTS, DUCT (Decoupled UCT)
  * **특징**: 상대방 수를 고려한 최적수 탐색. 게임 종료 상태 판별 및 평가 함수 설계 핵심.
<br>

### 2. 모듈화된 게임 환경 설계 (`GameState`)

다양한 게임에 알고리즘을 쉽게 적용할 수 있도록 `GameState` 추상 클래스(`common/game_state.h`)를 정의하고, 각 게임이 이를 상속받아 구현했습니다.

* **핵심 인터페이스**:
  * `is_done()`: 게임 종료 여부 반환.
  * `get_legal_actions()`: 현재 상태에서 가능한 행동 목록 반환.
  * `advance(action)` / `advance_sim(action1, action2)`: 행동을 통해 다음 상태로 전이.
  * `evaluate_score()`: 현재 상태의 점수 (또는 승률 예측) 반환.
  * `to_string()`: 현재 게임 보드 상태를 문자열로 시각화.

* **구현된 게임 (`games/`)**:
  * `maze`: 기본적인 경로 찾기 미로.
  * `simmaze` (Simultaneous Maze): 두 플레이어가 동시에 움직이는 미로.
  * `twomaze`: 두 플레이어가 각자의 미로에서 점수를 얻는 게임.
  * `wallmaze`: 벽을 설치하며 상대방을 방해하는 미로 게임 (비트셋 최적화 버전 포함).
  * `automaze`: 자동으로 벽이 생성되는 미로.
  * `connect_four`: 4목 게임 (비트보드 최적화 버전 포함).
<br>

### 3. 성능 최적화: 커넥트 포 비트보드 (`connect_four_bitboard.h`)

* **도전**: 커넥트 포 게임은 상태 공간이 크고, 승리 조건 확인(가로, 세로, 대각선 4개 연결) 연산이 빈번합니다.
* **해결**: 6x7 보드를 64비트 정수 2개(`myBoard`, `enemyBoard`)로 표현하는 **비트보드**를 사용했습니다. 각 비트는 해당 위치에 돌이 있는지 여부를 나타냅니다.
* **효과**:
  * **상태 저장/복사**: 64비트 정수 2개만 복사하면 되므로 MCTS 등 상태 복사가 빈번한 알고리즘에서 매우 효율적입니다.
  * **승리 조건 확인**: 비트 연산(shift, AND, OR)을 통해 여러 방향의 연결 여부를 병렬적으로 빠르게 확인할 수 있습니다. 예를 들어, `(board & (board >> 7))`과 같은 연산으로 대각선 연결을 효율적으로 검사합니다.
  * **수 놓기/가져오기**: 특정 열에 돌을 놓는 연산(가장 아래 빈칸 찾기)과 특정 위치의 돌 유무 확인도 비트 연산으로 최적화됩니다.
<br>

### 4. 체계적인 벤치마킹 및 분석 (`benchmark.md`)

* **목표**: 각 알고리즘이 다양한 게임 환경에서 어떤 성능 특성(탐색 시간, 결과 점수/승률)을 보이는지 정량적으로 비교합니다.
* **방법**: 각 게임별 `*_benchmark` 실행 파일은 여러 알고리즘을 동일한 조건(예: 제한 시간)에서 실행하고 결과를 출력합니다. `benchmark.md`는 이 결과들을 취합하여 표와 그래프로 시각화하고 분석합니다.
* **결과 예시**:
  * 커넥트 포에서는 **MCTS와 Iterative Deepening (Alpha-Beta 기반)이 가장 강력한 성능**을 보였습니다. 특히 비트보드 최적화 버전(`bit_mcts`, `bit_iterative_deepening`)은 일반 버전에 비해 훨씬 빠른 속도를 나타냈습니다.
  * 미로 탐색에서는 **A*가 일반적으로 우수한 성능**을 보였으나, 휴리스틱 함수의 정확도에 따라 성능이 달라질 수 있습니다.
  * 싱글 플레이어 게임에서는 **빔 탐색 계열(Beam Search, Chokudai)이 시간 제약 하에서 좋은 결과**를 찾는 데 효과적이었습니다.

---
<br>

## 📊 결과 및 성과 (Result)

* **포괄적인 알고리즘 구현**: 경로 탐색부터 싱글/2인 플레이어 게임 전략까지 **15개 이상의 핵심 AI 탐색 알고리즘**을 C++로 구현했습니다.
* **정량적 성능 데이터**: 다양한 게임 환경에서 각 알고리즘의 성능을 측정한 **벤치마크 결과**(`docs/benchmark.md`)를 제공하여, 이론적 개념과 실제 성능 간의 관계를 명확히 보여줍니다.
* **코드 재사용성 및 확장성**: `GameState` 추상화 및 `AlgorithmFactory`(`algorithm_factory.cpp`)를 통해 새로운 게임이나 알고리즘을 비교적 쉽게 추가하고 테스트할 수 있는 구조를 갖추었습니다.
* **성능 최적화 시연**: 커넥트 포 게임에서 **비트보드**를 적용하여 알고리즘 실행 속도를 크게 향상시킨 사례를 통해 C++ 기반 성능 최적화 역량을 보여줍니다.

---
<br>

## 💡 주요 학습 포인트 (Learnings) (`reflections.md` 기반)

* **알고리즘 선택의 중요성**: 문제 유형(경로 탐색, 제로섬 게임 등)과 제약 조건(시간 제한, 메모리)에 따라 최적의 알고리즘이 달라짐을 벤치마크를 통해 실증적으로 확인했습니다. 예를 들어, MCTS는 평가 함수가 불분명해도 강력하지만 계산 비용이 높고, Alpha-Beta는 좋은 평가 함수가 있을 때 매우 효율적입니다.
* **C++ 구현의 이점**: 저수준 메모리 관리와 비트 연산 등을 통해 알고리즘의 성능을 극대화할 수 있었습니다 (예: 비트보드).
* **평가 함수의 중요성**: Minimax, Alpha-Beta, Beam Search 등 휴리스틱 기반 알고리즘의 성능은 평가 함수의 설계에 크게 의존함을 확인했습니다.
* **벤치마킹의 가치**: 이론적 분석만으로는 알기 어려운 실제 성능 차이와 특정 환경에서의 알고리즘 동작 특성을 파악하는 데 체계적인 벤치마킹이 필수적임을 경험했습니다.

---
<br>


## 🚀 기술 스택

* **언어**: C++ (C++11 이상 표준 활용)
* **핵심 라이브러리**: STL (Standard Template Library 등)
* **빌드 시스템**: `Makefile`
* **핵심 구현 기법**:
  * 객체 지향 설계 (`GameState`, `AlgorithmInterface` 추상 클래스)
  * Factory 패턴 (`AlgorithmFactory`)
  * 비트 연산 (Bitboard 최적화)
  * 재귀 및 반복 기반 탐색 (DFS, Minimax, MCTS 등)
  * 자료 구조 활용 (우선순위 큐 in A*, Dijkstra; 큐 in BFS; 스택 in DFS)
---
<br>


## 📁 프로젝트 구조
```
├── README.md
├── Makefile                  # 빌드 자동화
├── src/
│   ├── algorithms/           # 알고리즘 구현
│   │   ├── pathfinding/      # BFS, DFS, A*, Dijkstra
│   │   ├── single_player/    # Greedy, Beam Search, Hill Climbing 등
│   │   └── two_player/       # Minimax, MCTS, Alpha-Beta 등
│   ├── games/                # 게임 환경 구현
│   │   ├── maze/
│   │   ├── simmaze/
│   │   ├── wallmaze/
│   │   ├── automaze/
│   │   └── connect_four/
│   ├── common/               # GameState 추상 클래스
│   ├── examples/             # 데모 및 벤치마크 실행 파일
│   └── utils/                # 유틸리티 함수
├── bin/                      # 빌드된 실행 파일
└── docs/
    ├── benchmark.md          # 벤치마크 결과 및 분석
    └── reflections.md        # 학습 내용 정리
```
<br>

## 🚀 시작하기 (Getting Started)

### 전제 조건

* C++ 컴파일러 (g++ 권장, C++11 이상 지원)
* `make` 빌드 도구
<br>

### 빌드
```bash
# 저장소 클론
git clone https://github.com/ahfmrptekd/game_ai_search_algorithms.git
cd game_ai_search_algorithms

# 전체 프로젝트 빌드 (bin/ 디렉토리에 실행 파일 생성)
make all
# 또는 특정 게임 빌드 (예: 커넥트 포)
# make connect_four
```
<br>

### 데모 실행

`bin/` 디렉토리에서 원하는 데모 실행 파일을 실행합니다.
```bash
# 커넥트 포 데모 실행 (플레이어 vs AI)
./bin/connect_four_demo

# 기본 미로 탐색 데모 실행
./bin/maze_demo

# 동시 이동 미로 데모 실행
./bin/simmaze_demo

# 벽 설치 미로 데모 실행
./bin/wallmaze_demo

# 기타 데모 실행 파일...
./bin/automaze_demo
./bin/twomaze_demo
```
<br>

### 벤치마크 실행

`bin/` 디렉토리에서 원하는 벤치마크 실행 파일을 실행합니다. 결과는 터미널에 출력됩니다.
```bash
# 전체 벤치마크 실행 (시간이 오래 걸릴 수 있음)
make benchmark

# 또는 개별 벤치마크 실행
./bin/connect_four_benchmark > connect_four_results.txt
./bin/maze_benchmark > maze_results.txt
# ... 기타 벤치마크 실행 파일 ...
```

벤치마크 결과 해석 및 비교는 [`docs/benchmark.md`](docs/benchmark.md) 문서를 참조하세요.

---

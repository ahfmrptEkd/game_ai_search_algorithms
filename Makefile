# 컴파일러 및 플래그
CXX = g++
CXXFLAGS = -std=c++17

# 디렉토리 경로
SRCDIR = src
BINDIR = bin
GAMES_DIR = $(SRCDIR)/games
ALGO_DIR = $(SRCDIR)/algorithms/single_player
ALGO_DIR_2P = $(SRCDIR)/algorithms/two_player
COMMON_DIR = $(SRCDIR)/common
EXAMPLES_DIR = $(SRCDIR)/examples
ALGO_BASE_DIR = $(SRCDIR)/algorithms

# 공통 소스 파일
COMMON_SRC = $(COMMON_DIR)/game_util.cpp

# 알고리즘 팩토리 관련 소스 파일
ALGO_FACTORY_SRC = $(ALGO_BASE_DIR)/algorithm_factory.cpp

# 게임별 변수 정의 포함 (변수만 포함, 빌드 규칙 제외)
include $(GAMES_DIR)/maze/Makefile.inc
include $(GAMES_DIR)/automaze/Makefile.inc
include $(GAMES_DIR)/twomaze/Makefile.inc
include $(GAMES_DIR)/simmaze/Makefile.inc
include $(GAMES_DIR)/wallmaze/Makefile.inc
include $(GAMES_DIR)/connect_four/Makefile.inc
# 단일 플레이어 알고리즘 소스
SINGLE_PLAYER_ALGOS = $(wildcard $(ALGO_DIR)/with_context/*.cpp) \
                      $(wildcard $(ALGO_DIR)/without_context/*.cpp)

# 두 플레이어 알고리즘 소스
TWO_PLAYER_ALGOS = $(wildcard $(ALGO_DIR_2P)/alternate/*.cpp)

# 전체 소스 파일들
ALL_SOURCES = $(MAZE_SRC) $(AUTOMAZE_SRC) $(TWOMAZE_SRC) $(SIMMAZE_SRC) $(WALLMAZE_SRC) \
              $(MAZE_ALGO) $(AUTOMAZE_ALGO) $(TWOMAZE_ALGO) $(SIMMAZE_ALGO) $(WALLMAZE_ALGO) \
              $(PATHFINDING_SRC) $(COMMON_SRC) $(ALGO_FACTORY_SRC) $(CONNECT_FOUR_SRC)

# 사용 가능한 게임 목록
GAMES = maze automaze twomaze simmaze wallmaze connect_four
# 기본 타겟: 모든 게임 빌드
all: $(GAMES) twomaze_battle

# 개별 게임 타겟 정의
maze: $(BINDIR)/maze_demo
automaze: $(BINDIR)/automaze_demo
twomaze: $(BINDIR)/twomaze_demo
twomaze_battle: $(BINDIR)/twomaze_battle
simmaze: $(BINDIR)/simmaze_demo
wallmaze: $(BINDIR)/wallmaze_demo
connect_four: $(BINDIR)/connect_four_demo

# 벤치마크 타겟
BENCHMARK_TARGETS = maze_benchmark automaze_benchmark twomaze_benchmark simmaze_benchmark wallmaze_benchmark pathfinding_benchmark
benchmark: $(BENCHMARK_TARGETS)
maze_benchmark: $(BINDIR)/maze_benchmark
automaze_benchmark: $(BINDIR)/automaze_benchmark
twomaze_benchmark: $(BINDIR)/twomaze_benchmark
simmaze_benchmark: $(BINDIR)/simmaze_benchmark
wallmaze_benchmark: $(BINDIR)/wallmaze_benchmark
pathfinding_benchmark: $(BINDIR)/pathfinding_benchmark

# 클린 타겟
clean:
	@echo "Cleaning binaries..."
	@rm -rf $(BINDIR)
	@echo "Clean completed!"

# 테스트 타겟
test: all benchmark
	@echo "Running basic tests..."
	$(BINDIR)/maze_demo random
	$(BINDIR)/automaze_demo random
	$(BINDIR)/twomaze_demo random
	$(BINDIR)/simmaze_demo random
	$(BINDIR)/wallmaze_demo --mode play --algo random
	@echo "Basic tests completed!"

# 수정된 빌드 규칙
$(BINDIR)/maze_demo: $(EXAMPLES_DIR)/maze_demo.cpp $(ALL_SOURCES)
	@echo "Building maze game..."
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Maze game built successfully!"

$(BINDIR)/automaze_demo: $(EXAMPLES_DIR)/automaze_demo.cpp $(ALL_SOURCES)
	@echo "Building automaze game..."
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "AutoMaze game built successfully!"

$(BINDIR)/twomaze_demo: $(EXAMPLES_DIR)/twomaze_demo.cpp $(ALL_SOURCES)
	@echo "Building TwoMaze game..."
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "TwoMaze game built successfully!"

$(BINDIR)/simmaze_demo: $(EXAMPLES_DIR)/simmaze_demo.cpp $(ALL_SOURCES)
	@echo "Building SimMaze game..."
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "SimMaze game built successfully!"

$(BINDIR)/wallmaze_demo: $(EXAMPLES_DIR)/wallmaze_demo.cpp $(ALL_SOURCES)
	@echo "Building WallMaze game..."
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "WallMaze game built successfully!"

$(BINDIR)/maze_benchmark: $(EXAMPLES_DIR)/maze_benchmark.cpp $(ALL_SOURCES)
	@echo "Building maze benchmark..."
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Maze benchmark built successfully!"

$(BINDIR)/automaze_benchmark: $(EXAMPLES_DIR)/automaze_benchmark.cpp $(ALL_SOURCES)
	@echo "Building automaze benchmark..."
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "AutoMaze benchmark built successfully!"

$(BINDIR)/twomaze_benchmark: $(EXAMPLES_DIR)/twomaze_benchmark.cpp $(ALL_SOURCES)
	@echo "Building twomaze benchmark..."
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "TwoMaze benchmark built successfully!"

$(BINDIR)/twomaze_battle: $(EXAMPLES_DIR)/twomaze_battle.cpp $(ALL_SOURCES)
	@echo "Building TwoMaze AI battle..."
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "TwoMaze AI battle built successfully!"

$(BINDIR)/simmaze_benchmark: $(EXAMPLES_DIR)/simmaze_benchmark.cpp $(ALL_SOURCES)
	@echo "Building SimMaze benchmark..."
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "SimMaze benchmark built successfully!"

$(BINDIR)/wallmaze_benchmark: $(EXAMPLES_DIR)/wallmaze_benchmark.cpp $(ALL_SOURCES)
	@echo "Building WallMaze benchmark..."
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "WallMaze benchmark built successfully!"

$(BINDIR)/pathfinding_benchmark: $(EXAMPLES_DIR)/pathfinding_benchmark.cpp $(ALL_SOURCES)
	@echo "Building pathfinding benchmark..."
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Pathfinding benchmark built successfully!"

$(BINDIR)/connect_four_demo: $(EXAMPLES_DIR)/connect4_demo.cpp $(ALL_SOURCES)
	@echo "Building ConnectFour game..."
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "ConnectFour game built successfully!"

# 도움말
help:
	@echo "사용 가능한 타겟:"
	@echo "  make all            - 모든 게임 빌드"
	@echo "  make benchmark      - 모든 벤치마크 프로그램 빌드"
	@echo "  make test           - 기본 테스트 실행"
	@echo "  make clean          - 생성된 바이너리 파일 삭제"
	@echo "  make help           - 도움말 표시"
	@echo ""
	@echo "게임별 타겟:"
	@for game in $(GAMES); do \
		echo "  make $$game       - $$game 빌드"; \
	done
	@echo ""
	@echo "벤치마크 타겟:"
	@echo "  make maze_benchmark      - 미로 벤치마크 빌드"
	@echo "  make automaze_benchmark  - 자동 미로 벤치마크 빌드"
	@echo "  make twomaze_benchmark   - 2인 미로 벤치마크 빌드"
	@echo "  make simmaze_benchmark   - 동시 미로 벤치마크 빌드"
	@echo "  make wallmaze_benchmark  - 벽이 있는 미로 벤치마크 빌드"
	@echo "  make pathfinding_benchmark - 경로 탐색 알고리즘 벤치마크 빌드"

.PHONY: all $(GAMES) benchmark $(BENCHMARK_TARGETS) clean help test twomaze_battle
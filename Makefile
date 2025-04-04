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

# 단일 플레이어 알고리즘 소스
SINGLE_PLAYER_ALGOS = $(wildcard $(ALGO_DIR)/with_context/*.cpp) \
                      $(wildcard $(ALGO_DIR)/without_context/*.cpp)

# 두 플레이어 알고리즘 소스
TWO_PLAYER_ALGOS = $(wildcard $(ALGO_DIR_2P)/alternate/*.cpp) \

# 사용 가능한 게임 목록
GAMES = maze automaze twomaze

# 게임별 규칙 포함
include $(GAMES_DIR)/maze/Makefile.inc
include $(GAMES_DIR)/automaze/Makefile.inc
include $(GAMES_DIR)/twomaze/Makefile.inc

# 기본 타겟: 모든 게임 빌드
all: $(GAMES)

# 벤치마크 타겟
benchmark: maze_benchmark automaze_benchmark twomaze_benchmark

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
	@echo "Basic tests completed!"

# 도움말
help:
	@echo "사용 가능한 타겟:"
	@echo "  make all         - 모든 게임 빌드"
	@echo "  make benchmark   - 모든 벤치마크 프로그램 빌드"
	@echo "  make test        - 기본 테스트 실행"
	@echo "  make clean       - 생성된 바이너리 파일 삭제"
	@echo "  make help        - 도움말 표시"
	@echo ""
	@echo "게임별 타겟:"
	@for game in $(GAMES); do \
		echo "  make $$game       - $$game 빌드"; \
	done
	@echo ""
	@echo "벤치마크 타겟:"
	@echo "  make maze_benchmark     - 미로 벤치마크 빌드"
	@echo "  make automaze_benchmark - 자동 미로 벤치마크 빌드"
	@echo "  make twomaze_benchmark  - 2인 미로 벤치마크 빌드"

.PHONY: all $(GAMES) benchmark maze_benchmark automaze_benchmark twomaze_benchmark clean help test
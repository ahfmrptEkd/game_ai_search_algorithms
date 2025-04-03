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
ALGO_FACTORY_SRC = $(ALGO_BASE_DIR)/algorithm_factory.cpp

# 사용 가능한 게임 목록
GAMES = maze automaze twomaze

# 게임별 규칙 포함
include $(GAMES_DIR)/maze/Makefile.inc
include $(GAMES_DIR)/automaze/Makefile.inc
include $(GAMES_DIR)/twomaze/Makefile.inc

# 기본 타겟: 모든 게임 빌드
all: $(GAMES)

# 벤치마크 타겟
benchmark: maze_benchmark

# 새로운 게임 추가 방법
# 1. GAMES 변수에 게임 이름 추가
# 2. src/games/[게임이름]/Makefile.inc 파일 생성
# 3. make [게임이름] 명령으로 빌드 가능

# 클린 타겟
clean:
	@echo "Cleaning binaries..."
	@rm -rf $(BINDIR)
	@echo "Clean completed!"

# 도움말
help:
	@echo "사용 가능한 타겟:"
	@echo "  make all         - 모든 게임 빌드"
	@echo "  make clean       - 생성된 바이너리 파일 삭제"
	@echo "  make help        - 도움말 표시"
	@echo ""
	@echo "게임별 타겟:"
	@for game in $(GAMES); do \
		echo "  make $$game       - $$game 빌드"; \
	done
	@echo ""
	@echo "기타 타겟:"
	@echo "  make benchmark   - 벤치마크 프로그램 빌드"

.PHONY: all $(GAMES) benchmark maze_benchmark clean help 
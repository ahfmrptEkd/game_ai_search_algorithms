#!/bin/bash

# 색상 정의
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 테스트 결과 카운터
TOTAL_TESTS=0
PASSED_TESTS=0

# 테스트 함수
function run_test() {
  TOTAL_TESTS=$((TOTAL_TESTS + 1))
  local name=$1
  local command=$2
  
  echo -e "${BLUE}[TEST]${NC} $name"
  echo "$ $command"
  
  if eval $command; then
    echo -e "${GREEN}[PASS]${NC} $name"
    PASSED_TESTS=$((PASSED_TESTS + 1))
  else
    echo -e "${RED}[FAIL]${NC} $name"
  fi
  echo
}

# 사전 체크
echo "===== 빌드 환경 확인 ====="
echo "컴파일러 버전:"
g++ --version
echo

# 프로젝트 빌드
echo "===== 프로젝트 빌드 ====="
run_test "전체 빌드" "make clean && make all"
run_test "벤치마크 빌드" "make benchmark"
run_test "Connect Four 빌드" "make connect_four"

# 기본 기능 테스트
echo "===== 기본 기능 테스트 ====="

# Maze 게임 테스트
for algo in random greedy beam chokudai
do
  run_test "Maze 게임 ($algo)" "./bin/maze_demo $algo"
done

# AutoMaze 게임 테스트
for algo in random hillclimb annealing
do
  run_test "AutoMaze 게임 ($algo)" "./bin/automaze_demo $algo"
done

# TwoMaze 게임 테스트
for algo in random minimax alphabeta deepening mc mcts thunder
do
  run_test "TwoMaze 게임 ($algo)" "./bin/twomaze_demo $algo"
done

# # Connect Four 게임 테스트
# for algo in ConnectFourRandom ConnectFourMCTS ConnectFourBitMCTS ConnectFourID ConnectFourBitID
# do
#   run_test "Connect Four 게임 ($algo)" "./bin/connect_four_demo $algo"
# done

# 벤치마크 간단 테스트
echo "===== 벤치마크 간단 테스트 ====="
run_test "Maze 벤치마크" "./bin/maze_benchmark random 2"
run_test "AutoMaze 벤치마크" "./bin/automaze_benchmark random 2 100"
run_test "TwoMaze 벤치마크" "./bin/twomaze_benchmark --games 2 --mode all"

# AI 대결 테스트
echo "===== AI 대결 테스트 ====="
run_test "Thunder vs AlphaBeta" "./bin/twomaze_battle --algo1 thunder --algo2 alphabeta --games 2"
# run_test "ConnectFourRandom vs ConnectFourMCTS" "./bin/connect_four_demo ConnectFourRandom ConnectFourMCTS"
# run_test "ConnectFourMCTS vs ConnectFourBitMCTS" "./bin/connect_four_demo ConnectFourMCTS ConnectFourBitMCTS"
# run_test "ConnectFourID vs ConnectFourBitID" "./bin/connect_four_demo ConnectFourID ConnectFourBitID"

# 결과 요약
echo "===== 테스트 결과 요약 ====="
echo -e "총 테스트: $TOTAL_TESTS"
echo -e "통과: ${GREEN}$PASSED_TESTS${NC}"
echo -e "실패: ${RED}$((TOTAL_TESTS - PASSED_TESTS))${NC}"

if [ $PASSED_TESTS -eq $TOTAL_TESTS ]; then
  echo -e "${GREEN}모든 테스트 통과!${NC}"
  exit 0
else
  echo -e "${RED}일부 테스트 실패${NC}"
  exit 1
fi
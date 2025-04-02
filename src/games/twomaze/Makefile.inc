# TwoMaze 게임 소스 파일
TWOMAZE_SRC := $(GAMES_DIR)/twomaze/twomaze_state.cpp

# TwoMaze 알고리즘 파일
TWOMAZE_ALGO := $(ALGO_DIR_2P)/alternate/random.cpp \
				$(ALGO_DIR_2P)/alternate/minimax.cpp \
				$(ALGO_DIR_2P)/alternate/alphabeta.cpp \
				$(ALGO_DIR_2P)/alternate/deepening.cpp \
				$(ALGO_DIR_2P)/alternate/mc.cpp \
				$(ALGO_DIR_2P)/alternate/mcts.cpp \
				$(ALGO_DIR_2P)/alternate/thunder.cpp \
				

				
				

# TwoMaze 게임 빌드 규칙
twomaze: $(BINDIR)/twomaze_demo

$(BINDIR)/twomaze_demo: $(EXAMPLES_DIR)/twomaze_demo.cpp $(TWOMAZE_SRC) $(TWOMAZE_ALGO) $(COMMON_SRC)
	@echo "Building TwoMaze game..."
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "TwoMaze game built successfully!"

# TwoMaze 벤치마크 빌드 규칙
twomaze_benchmark: $(BINDIR)/twomaze_benchmark

$(BINDIR)/twomaze_benchmark: $(EXAMPLES_DIR)/twomaze_benchmark.cpp $(TWOMAZE_SRC) $(TWOMAZE_ALGO) $(COMMON_SRC)
	@echo "Building TwoMaze benchmark..."
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "TwoMaze benchmark built successfully!"

# 2인용 게임 AI 대결 정의
twomaze_battle: $(BINDIR)/twomaze_battle

$(BINDIR)/twomaze_battle: $(EXAMPLES_DIR)/twomaze_battle.cpp $(TWOMAZE_SRC) $(TWOMAZE_ALGO) $(COMMON_SRC)
	@echo "Building TwoMaze AI battle..."
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "TwoMaze AI battle built successfully!"

# Maze 게임 소스 파일
MAZE_SRC := $(GAMES_DIR)/maze/maze_state.cpp
MAZE_ALGO := $(ALGO_DIR)/with_context/random.cpp \
             $(ALGO_DIR)/with_context/greedy.cpp \
             $(ALGO_DIR)/with_context/beam.cpp \
             $(ALGO_DIR)/with_context/chokudai.cpp

# Maze 게임 빌드 규칙
maze: $(BINDIR)/maze_demo

$(BINDIR)/maze_demo: $(EXAMPLES_DIR)/maze_demo.cpp $(MAZE_SRC) $(MAZE_ALGO) $(COMMON_SRC)
	@echo "Building maze game..."
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Maze game built successfully!"

# Maze 벤치마크 빌드 규칙
maze_benchmark: $(BINDIR)/maze_benchmark

$(BINDIR)/maze_benchmark: $(EXAMPLES_DIR)/maze_benchmark.cpp $(MAZE_SRC) $(MAZE_ALGO) $(COMMON_SRC)
	@echo "Building maze benchmark..."
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Maze benchmark built successfully!" 
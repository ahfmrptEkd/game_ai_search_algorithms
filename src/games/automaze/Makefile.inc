# AutoMaze 게임 소스 파일
AUTOMAZE_SRC := $(GAMES_DIR)/automaze/automaze_state.cpp
AUTOMAZE_ALGO := $(ALGO_DIR)/without_context/random.cpp \
				$(ALGO_DIR)/without_context/hillclimb.cpp \
				$(ALGO_DIR)/without_context/simulated_annealing.cpp

# AutoMaze 게임 빌드 규칙
automaze: $(BINDIR)/automaze_demo

$(BINDIR)/automaze_demo: $(EXAMPLES_DIR)/automaze_demo.cpp $(AUTOMAZE_SRC) $(AUTOMAZE_ALGO) $(COMMON_SRC) $(ALGO_FACTORY_SRC)
	@echo "Building automaze game..."
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "AutoMaze game built successfully!" 

# AutoMaze 벤치마크 빌드 규칙
automaze_benchmark: $(BINDIR)/automaze_benchmark

$(BINDIR)/automaze_benchmark: $(EXAMPLES_DIR)/automaze_benchmark.cpp $(AUTOMAZE_SRC) $(AUTOMAZE_ALGO) $(COMMON_SRC) $(ALGO_FACTORY_SRC)
	@echo "Building automaze benchmark..."
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "AutoMaze benchmark built successfully!"
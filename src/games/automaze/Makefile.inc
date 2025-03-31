# AutoMaze 게임 소스 파일
AUTOMAZE_SRC := $(GAMES_DIR)/automaze/automaze_state.cpp
AUTOMAZE_ALGO := $(ALGO_DIR)/without_context/random.cpp

# AutoMaze 게임 빌드 규칙
automaze: $(BINDIR)/automaze_demo

$(BINDIR)/automaze_demo: $(EXAMPLES_DIR)/automaze_demo.cpp $(AUTOMAZE_SRC) $(AUTOMAZE_ALGO) $(COMMON_SRC)
	@echo "Building automaze game..."
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "AutoMaze game built successfully!" 
# Maze 게임 소스 파일
MAZE_SRC := $(GAMES_DIR)/maze/maze_state.cpp
MAZE_ALGO := $(ALGO_DIR)/with_context/random.cpp \
             $(ALGO_DIR)/with_context/greedy.cpp \
             $(ALGO_DIR)/with_context/beam.cpp \
             $(ALGO_DIR)/with_context/chokudai.cpp
# SimMaze 게임 소스 파일
SIMMAZE_SRC := $(GAMES_DIR)/simmaze/simmaze_state.cpp

# SimMaze 알고리즘 파일
SIMMAZE_ALGO := $(ALGO_DIR_2P)/simultaneous/random.cpp \
                $(ALGO_DIR_2P)/simultaneous/pmc.cpp \
                $(ALGO_DIR_2P)/simultaneous/mcts_sim.cpp \
                $(ALGO_DIR_2P)/simultaneous/duct.cpp
# WallMaze 게임 소스 파일
WALLMAZE_SRC := $(GAMES_DIR)/wallmaze/wallmaze_state.cpp \
                $(GAMES_DIR)/wallmaze/zobrist_hash.cpp \
                $(GAMES_DIR)/wallmaze/wallmaze_bitset.cpp \
                $(GAMES_DIR)/wallmaze/wallmaze_bitset_state.cpp \


# Pathfinding 알고리즘 소스 파일
PATHFINDING_SRC := $(ALGO_BASE_DIR)/pathfinding/pathfinding.cpp \
                  $(ALGO_BASE_DIR)/pathfinding/bfs.cpp \
                  $(ALGO_BASE_DIR)/pathfinding/dfs.cpp \
                  $(ALGO_BASE_DIR)/pathfinding/dijkstra.cpp \
                  $(ALGO_BASE_DIR)/pathfinding/astar.cpp
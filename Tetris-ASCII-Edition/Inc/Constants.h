#pragma once
#include <vector>
#include <string>

// macros
#define SAMPLE_BOARD(array, x, y) ((array)[(x) + (BOARD_W) * (y)])

// constants
#define MAX_LEVEL 10

#define GAME_TICK 5			//ms
#define BASE_SPEED 70		// ticks
#define LEVEL_DECR floor(BASE_SPEED/MAX_LEVEL)		// ticks

#define BOARD_W 10
#define BOARD_H 20
#define UI_UNIT_W 10

#define TETROMINO_COUNT 7
#define TETROMINO_SIZE 16
#define TETROMINO_W 4

#define GHOST_SYM '.'		// used for displaying the ghost piece

// (level 1-3) points for:
// { dropping pieces | 1 lines | 2 lines | 3 lines | 4 lines }
static const int SCORE_DEF[5] = { 1, 100, 400, 900, 2000 };		// base values used for calculating scores

// externs
extern std::vector<std::string> menu_labels;
extern std::vector<std::string> settings_labels;
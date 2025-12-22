#pragma once

// macros
#define SAMPLE_BOARD(array, x, y) ((array)[(x) + (BOARD_W) * (y)])
#define MS_TO_TICKS(duration) (static_cast<uint16_t>(floor((duration) / (GAME_TICK)) / 3.0))

// constants
#define MAX_LEVEL 10

#define GAME_TICK 5			//ms
#define BASE_SPEED 100		// ticks
#define LEVEL_DECR 10		// ticks

#define BOARD_W 10
#define BOARD_H 20
#define UI_UNIT_W 10

#define TETROMINO_COUNT 7
#define TETROMINO_SIZE 16
#define TETROMINO_W 4

#define GHOST_SYM '.'		// used for displaying the ghost piece

// base values used for calculating scores
// (level 1-3) points for:
// hard droping pieces | 1 lines | 2 lines | 3 lines | 4 lines
static const int SCORE_DEF[5] = { 1, 100, 400, 900, 2000 };

// default settings
struct GameSettings {
	uint8_t start_level = 1;
	bool ascii_mode = false;
};
#pragma once

#define MAX_LEVEL 10

#define GAME_TICK 10		//ms
#define BASE_SPEED 100		// ticks
#define LEVEL_DECR 10		// ticks


#define BOARD_W 10
#define BOARD_H 20

#define TETROMINO_COUNT 7
#define TETROMINO_SIZE 16
#define TETROMINO_W 4

struct GameSettings {		// default settings
	uint8_t start_level = 1;
	bool ascii_mode = false;
};
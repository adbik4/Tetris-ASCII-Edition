#pragma once

#define GAME_TICK 10	//ms

#define BOARD_W 10
#define BOARD_H 20

#define TETROMINO_COUNT 7
#define TETROMINO_SIZE 16
#define TETROMINO_W 4

struct GameSettings {
	uint8_t start_level = 0;
	bool ascii_mode = false;
};
#pragma once
#include <cstdint>
#include <array>
#include <memory>
#include "Tetromino.h"
#include "Constants.h"

class GameState {
public:
	// engine state
	bool running;		// used to check if the engine systems are running
	bool stop_flag;		// triggers the app shutdown
	bool ascii_mode;	// controls the rendering mode
	uint64_t tick;		// stores the current tick no

	// game variables
	uint8_t level;
	uint64_t score;

	std::array<char, BOARD_W * BOARD_H> board;
	Tetromino active_piece;
	Tetromino ghost_piece;

	// initial conditions
	GameState(const GameSettings& cfg) {
		running = false;
		stop_flag = false;
		score = 0;
		tick = 1;
		board.fill('.');

		level = cfg.start_level;
		ascii_mode = cfg.ascii_mode;
	}
};
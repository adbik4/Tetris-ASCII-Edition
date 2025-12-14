#pragma once
#include <cstdint>
#include <string>
#include <memory>
#include "Tetromino.h"
#include "Constants.h"

class GameState {
public:
	// engine state
	bool running;	// used to check if the engine systems are running
	bool stop_flag; // triggers the app shutdown
	uint64_t frame;	// stores the current frame no

	// game variables
	uint8_t level;
	uint64_t score;

	std::string board;
	Tetromino active_piece;

	// initial conditions
	GameState(const GameSettings& cfg) {
		running = false;
		stop_flag = false;
		level = cfg.start_level;
		score = 0;
		frame = 0;
		board = std::string(BOARD_W * BOARD_H, '.');
	}
};
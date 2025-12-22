#pragma once
#include <cstdint>
#include <array>
#include <memory>
#include "Tetromino.h"
#include "Constants.h"

class GameState {
public:
	// engine state
	bool running;			// used to check if the engine systems are running
	bool stop_flag;			// triggers the app shutdown
	bool ascii_mode;		// controls the rendering mode
	bool flash_on_clear;	// decides if the screen should flash after a line clear
	uint64_t tick;			// stores the current tick no
	uint64_t wait_until;	// used for in-game delays

	// game variables
	uint8_t level;
	uint64_t score;
	uint64_t top_score;
	uint16_t lines;

	std::array<char, BOARD_W * BOARD_H> board;
	Tetromino active_piece;
	Tetromino ghost_piece;

	// initial conditions
	GameState(const GameSettings& cfg) {
		level = cfg.start_level;
		ascii_mode = cfg.ascii_mode;
		flash_on_clear = cfg.flash_on_clear;

		board.fill('.');
		tick = 1;
		running = false;
		wait_until = 0;
		stop_flag = false;

		top_score = 694202137;
		score = 0;
		lines = 0;
	}
};
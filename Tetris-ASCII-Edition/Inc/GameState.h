#pragma once
#include <cstdint>
#include <array>
#include <memory>
#include "Tetromino.h"
#include "Constants.h"
#include "Settings.h"

class GameState {
public:
	// engine state
	bool running;			// used to check if the engine systems are running
	bool stop_flag;			// triggers the app shutdown
	bool ascii_mode;		// controls the rendering mode
	bool flash_on_clear;	// decides if the screen should flash after a line clear
	bool pure_randomness;	// decides how the pieces are randomized
	uint8_t start_level;	// stores the start_level setting
	uint64_t tick;			// stores the current tick no
	uint64_t wait_until;	// used for in-game delays

	// game variables
	uint8_t level;
	uint64_t score;
	uint64_t hi_score;
	uint16_t lines;
	uint16_t line_goal;

	std::array<char, BOARD_W * BOARD_H> board;
	Tetromino active_piece;
	Tetromino ghost_piece;

	// initial conditions
	GameState(const GameSettings& cfg) {
		start_level = level = cfg.start_level;
		ascii_mode = cfg.ascii_mode;
		flash_on_clear = cfg.flash_on_clear;
		pure_randomness = cfg.pure_randomness;
		hi_score = cfg.hi_score;

		if (cfg.hi_score > UINT64_MAX) {
			hi_score = 0;
		}
		if (cfg.start_level > MAX_LEVEL) {
			level = MAX_LEVEL;
		}

		board.fill('.');
		tick = 1;
		running = false;
		wait_until = 0;
		stop_flag = false;

		score = 0;
		lines = 0;
		line_goal = 10 * level;
	}

	void reset() {
		level = start_level;

		board.fill('.');
		tick = 1;
		running = false;
		wait_until = 0;
		stop_flag = false;

		score = 0;
		lines = 0;
		line_goal = 10 * level;
	}
};
#pragma once
#include <cstdint>
#include <array>
#include <memory>
#include "Tetromino.h"
#include "Constants.h"
#include "Settings.h"

#define GAME 0
#define MENU 1
#define SETTINGS 2

class GameState {
public:
	// engine state
	bool running;			// used to check if the engine systems are running
	bool stop_flag;			// triggers the app shutdown
	bool game_over;			// remembers the game over state
	bool ascii_mode;		// controls the rendering mode
	bool flash_on_clear;	// decides if the screen should flash after a line clear
	bool pure_randomness;	// decides how the pieces are randomized
	uint8_t active_window;	// keeps track of the window the game is in
	int8_t active_label;	// keeps track of the label selected by the user
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

		active_window = MENU;
		active_label = 0;
		board.fill('.');
		tick = 1;
		wait_until = 0;
		running = false;
		stop_flag = false;
		game_over = false;
		ghost_piece.is_ghost = true;

		score = 0;
		lines = 0;
		line_goal = 10 * level;
	}

	void reset() {
		active_window = GAME;
		level = start_level;

		board.fill('.');
		tick = 1;
		wait_until = 0;
		running = false;
		game_over = false;
		stop_flag = false;

		score = 0;
		lines = 0;
		line_goal = 10 * level;
	}
};
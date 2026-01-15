#pragma once
#include <cstdint>
#include <array>
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
	uint64_t tick;			// stores the current tick no
	uint64_t wait_until;	// used for in-game delays

	// settings
	GameSettings cfg;

	// menu state
	uint8_t active_window;	// keeps track of the window the game is in
	int8_t active_label;	// keeps track of the label selected by the user

	// game variables
	bool game_over;
	uint8_t level;
	uint64_t score;
	uint16_t lines;
	uint16_t line_goal;

	std::array<char, BOARD_W * BOARD_H> board;
	Tetromino active_piece;
	Tetromino next_piece;
	Tetromino ghost_piece;

	// initial conditions
	GameState(const GameSettings& cfg) : cfg(cfg) {
		level = cfg.start_level;

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
		level = cfg.start_level;

		board.fill('.');
		tick = 1;
		running = false;
		game_over = false;
		wait_until = 0;
		stop_flag = false;

		score = 0;
		lines = 0;
		line_goal = 10 * level;
	}
};
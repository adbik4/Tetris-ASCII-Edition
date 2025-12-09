#pragma once
#include <cstdint>

struct GameSettings {
	uint8_t start_level = 0;
};

class GameState {
public:
	// engine state
	bool running;	// used to check if the engine systems are running
	bool stop_flag; // triggers the app shutdown
	uint64_t frame;	// stores the current frame no

	// game variables
	uint8_t level;
	uint64_t score;
	uint8_t curr_piece;

	// initial conditions
	GameState(const GameSettings& cfg) {
		running = false;
		stop_flag = false;
		level = cfg.start_level;
		score = 0;
		curr_piece = 0;
		frame = 0;
	}
};
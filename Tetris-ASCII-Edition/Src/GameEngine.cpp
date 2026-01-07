#include <exception>
#include <fstream>
#include "GameEngine.h"
#include "Tetromino.h"
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

int int_input;

void GameEngine::startEngine() {
	state->running = true;

	try {
		renderer->showMenu();

		switch (int_input) {
		case 1:
			renderer->initGameUI();
			time_mgr->startClock();
			break;
		case 2:
			renderer->initSettingsUI();
			renderer->windowPrint(MAIN_MENU, "SETTINGS\n");
			break;
		case 3:
			state->stop_flag = true;
			break;
		}
	}
	catch (const std::exception& err) {
		string message = string("[DEBUG]: ") + err.what() + "\n";
		renderer->errPrint(message);
	}
	catch (...) {
      renderer->errPrint("[DEBUG]: Unknown exception\n");
	}
}

uint8_t target_x = 255, target_y = 0;

void GameEngine::update() {

	try {
		if (state->active_piece.is_falling) {
			state->active_piece.soft_drop(state->board);
		} 
		else {
			// ==== INPUT ====
			inputHandling();

			// ==== GAME LOGIC ====
			gameLogic();
		}

		// ==== RENDER OUTPUT ====
		renderer->renderFrame();
		renderer->refreshGameUI();
	}
	catch (const std::exception& err) {
		string message = string("[DEBUG]: ") + err.what() + ". Press [any key] to continue";
		renderer->errPrint(message);
		input_mgr->waitForAnyKey();
		state->stop_flag = true;
	}
}

// This function conducts the main mediation logic
void GameEngine::notify (const Event& event) {
	switch (event.id) {
	case EventId::CLK:
		update();
		state->tick++;
		break;
	case EventId::INPUT_ERR:
		renderer->windowPrint(INPUT_WIN, "Invalid input\n");
		break;
	case EventId::INT_INPUT:
		 int_input = input_mgr->getIntInput(event.args);
		break;
	case EventId::GENERAL_ERR:
		renderer->errPrint("[DEBUG] an error has occured\n");
		break;
	default:
		renderer->errPrint("[DEBUG] GameEngine has recieved an undefined event\n");
		break;
	}
}

void GameEngine::inputHandling() {
	int k_input = input_mgr->getKeyboardInput(GAME_WIN);
	switch (k_input) {
	case (int)'q':
	case (int)'j':
		state->active_piece.rotateL(state->board);
		break;

	case (int)'e':
	case (int)'k':
		state->active_piece.rotateR(state->board);
		break;

	case (int)'w':
	case KEY_UP:
		state->active_piece.hard_drop(state->board);
		break;

	case (int)'a':
	case KEY_LEFT:
		state->active_piece.moveL(state->board);
		break;

	case (int)'s':
	case KEY_DOWN:
		state->active_piece.soft_drop(state->board);
		break;

	case (int)'d':
	case KEY_RIGHT:
		state->active_piece.moveR(state->board);
		break;

	case 27: // ESC
		stopEngine();
	}
}

// Returns a random piece index using the TETRIS TGM3 algorithm.
// the game keeps track of a piece_bag with 35 pieces, initialised with 5 counts of each variant.
// to mitigate floods, it also tracks the last four pieces that were played.
// it will take a piece from the piece_bag at random, and check (up to six times) if it was recently played
// to prevent droughts, the missing spot in the piece_bag will get filled by the most needed piece
uint8_t GameEngine::TGM3_randomizer() {
	uint8_t piece, tmp;
	uint16_t random_idx;

	// roll six times
	for (uint8_t _ = 0; _ < 6; _++) {
		random_idx = static_cast<uint8_t>(uniform_int_distribution<int16_t>(0, 34)(rng));
		piece = piece_bag[random_idx];
		if (find(history.begin(), history.end(), piece) == history.end()) {
			break;
		}
	}

	// manage the drought history
	auto idx = find(drought_history.begin(), drought_history.end(), piece);
	if (idx == drought_history.end()) {
		drought_history.push_front(piece);
	}
	else {
		// update drought history
		tmp = *idx;
		drought_history.erase(idx);
		drought_history.push_front(tmp);
	}

	// replace the gap with the least used piece
	if (random_idx < 35) {
		piece_bag[random_idx] = drought_history.back();
	}

	// update history
	history.push_front(piece);
	history.pop_back();

	return piece;
}

// Returns a purely random piece without messing around
uint8_t GameEngine::pure_randomizer() {
	return static_cast<uint8_t>(uniform_int_distribution<int16_t>(1, 7)(rng));
}


void GameEngine::stopEngine() {
	if (!state->stop_flag) {
		state->stop_flag = true;
		return;
	}
	else if (!state->running) {
		return;
	}

	state->running = false;
	time_mgr->stopClock();
}
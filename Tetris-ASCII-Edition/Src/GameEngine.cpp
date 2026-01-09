#include <exception>
#include <fstream>
#include "GameEngine.h"
#include "Tetromino.h"
#include "SaveSystem.h"
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

// This function conducts the main mediation logic
void GameEngine::notify(const Event& event) {
	switch (event.id) {
	case EventId::CLK:
		update();
		state->tick++;
		break;
	case EventId::INPUT_ERR:
		renderer->windowPrint(INPUT_WIN, "Invalid input\n");
		break;
	case EventId::INT_INPUT:
		//int_input = input_mgr->getIntInput(event.args);
		break;
	case EventId::GENERAL_ERR:
		renderer->errPrint("[DEBUG] an error has occured\n");
		break;
	default:
		renderer->errPrint("[DEBUG] GameEngine has recieved an undefined event\n");
		break;
	}
}

void GameEngine::startEngine() {
	state->running = true;
	renderer->initMenuUI();
	time_mgr->startClock();
}

void GameEngine::update() {
	try {

		// ==== INPUT ====
		int k_input = input_mgr->getKeyboardInput();

		// ==== GAME LOGIC ====
		switch (state->active_window) {
			case GAME:
				gameLogic(k_input);
				break;
			case MENU:
				menuLogic(k_input);
				break;
			case SETTINGS:
				settingsLogic(k_input);
				break;
			default:
				throw std::domain_error("<update> unknown window");
		}

		// ==== RENDER OUTPUT ====
		switch (state->active_window) {
		case GAME:
			renderer->renderFrame();
			renderer->refreshGameUI();
			break;
		case MENU:
			renderer->refreshMenuUI();
			break;
		case SETTINGS:
			renderer->refreshSettingsUI();
			break;
		default:
			throw std::domain_error("<update> unknown window");
		}

	}
	catch (const std::exception& err) {
		if (err.what() == "<realize_piece> Tetromino tile index out of range") {
			gameOver();
		}
		string message = string("[DEBUG]: ") + err.what() + ". Press [any key] to continue";
		renderer->errPrint(message);
		input_mgr->waitForAnyKey();
		state->stop_flag = true;
	}
}

void GameEngine::restartGame() {
	state->reset();
	state->active_piece = Tetromino();
	renderer->initGameUI();
	renderer->windowReset(GAME_WIN);
}

void GameEngine::gameOver() {
	if (state->stop_flag) {
		return;
	}

	state->game_over = true;

	if (state->score > state->hi_score) {
		state->hi_score = state->score;
	}

	saveState(getState());
	renderer->renderFrame();

	for (auto i = 0; i < 3; i++) {
		this_thread::sleep_for(chrono::milliseconds(100));
		renderer->flashEffect();
	}

	renderer->showEndScreen(getState());
	int k_input = input_mgr->waitForAnyKey();

	switch (k_input) {
	case 27: // ESC
		state->active_label = 0;
		renderer->initMenuUI();
		state->active_window = MENU;
		return;
	}

	restartGame();
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
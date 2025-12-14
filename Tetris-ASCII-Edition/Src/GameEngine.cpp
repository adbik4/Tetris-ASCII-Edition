#include <exception>
#include <random>
#include "GameEngine.h"
#include "Tetromino.h"

using namespace std;

int int_input;
mt19937 rng((uint8_t) chrono::system_clock::now().time_since_epoch().count());

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

void GameEngine::update() {
	try {
		// INPUT
		int k_input = input_mgr->getKeyboardInput(GAME_WIN);
		switch (k_input) {
		case KEY_LEFT:
			state->active_piece.rotateL();
			break;
		case KEY_RIGHT:
			state->active_piece.rotateR();
			break;
		}

		// GAME LOGIC
		if (state->active_piece.get_piece_id() == NULL) {
			state->active_piece.next_piece(rng);
		}

		// RENDER OUTPUT
		renderer->renderFrame();
	}
	catch (const std::exception& err) {
		string message = string("[DEBUG]: ") + err.what() + "\n";
		renderer->errPrint(message);
		while (true) {};
	}
}

void GameEngine::stopEngine() {
	if (!state->running) return;
	state->running = false;
	renderer->showEndScreen();
	time_mgr->stopClock();
}

void GameEngine::nextLevel() {
	state->level++;
}

// This function conducts the main mediation logic
void GameEngine::notify (const Event& event) {
	switch (event.id) {
	case CLK:
		update();
		state->tick++;
		break;
	case INPUT_ERR:
		renderer->windowPrint(INPUT_WIN, "Invalid input\n");
		break;
	case INT_INPUT:
		int_input = input_mgr->getIntInput(event.args);
		break;
	case GENERAL_ERR:
		renderer->errPrint("[DEBUG] an error has occured\n");
		break;
	default:
		renderer->errPrint("[DEBUG] GameEngine has recieved an undefined event\n");
		break;
	}
}
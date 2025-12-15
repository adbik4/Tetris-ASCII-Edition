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

void GameEngine::update() {
	try {
		// INPUT
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
				//state->active_piece.hard_drop(state->board);
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

			case (int)'x':
				stopEngine();
		}

		// GAME LOGIC
		// gravity
		uint16_t drop_interval = BASE_SPEED - ((state->level - 1) * LEVEL_DECR);
		if (!drop_interval) {
			throw std::invalid_argument("Level got too high");
		}
		if (state->tick % drop_interval == 0) {
			state->active_piece.soft_drop(state->board);
		}

		// pieces get refreshed if needed
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
	if (!state->stop_flag) {
		state->stop_flag = true;
		return;
	}
	else if (!state->running) {
		return;
	}

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
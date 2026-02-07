#include <exception>
#include "GameEngine.h"

using namespace std;

// This function conducts the asynchronous logic in the program.
// Args: event - const Event& to act upon and/or dispatch to the engine components
// Returns: void
void GameEngine::notify(const Event& event) {
	switch (event) {
		case Event::CLK:
			update();
			state->tick++;
			break;
		default:
			renderer->errPrint("[DEBUG] GameEngine has recieved an undefined event\n");
			break;
	}
}

// Starts the GameEngine
// Args: none
// Returns: void
void GameEngine::startEngine() {
	renderer->initMenuUI();
	time_mgr->startClock();
	state->running = true;
}

// Synchronous update loop that's the heart of the program.
// Called after the GameEngine receives an EventId::CLK tick every GAME_TICK ms. 
// Args: none (uses internal state and managers)
// Returns: void
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
			if (state->cfg.ascii_mode) {
				renderer->renderASCIIFrame();
			} else {
				renderer->renderColorFrame();
			}
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

// Stops the GameEngine
// Args: none
// Returns: void
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
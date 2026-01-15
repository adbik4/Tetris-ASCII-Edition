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
	renderer->initMenuUI();
	time_mgr->startClock();
	state->running = true;
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
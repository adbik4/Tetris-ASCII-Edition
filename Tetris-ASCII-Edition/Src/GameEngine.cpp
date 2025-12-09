#include "GameEngine.h"

using namespace std;

int int_input;

void GameEngine::startGame() {
	running_ = true;

	renderer->showMenu();
	switch (int_input) {
	case 1:
		renderer->windowPrint(MAIN_MENU, "GAMEPLAY\n");
		time_mgr->startClock();
		break;
	case 2:
		renderer->windowPrint(MAIN_MENU, "SETTINGS\n");
		break;
	case 3:
		stop_flag = true;
		break;
	}
}

void GameEngine::update() {
	renderer->windowPrint(MAIN_MENU, "gameplay\n");
}

void GameEngine::stopGame() {
	if (!running_) return;
	running_ = false;
	renderer->showEndScreen();
	time_mgr->stopClock();
}

void GameEngine::nextLevel() {
	level_++;
}

// This function conducts the main mediation logic
void GameEngine::notify (const Event& event) {
	switch (event.id) {
	case CLK:
		update();
		break;
	case INPUT_ERR:
		renderer->windowPrint(INPUT_WIN, "Invalid input - try again...\n");
		break;
	case INT_INPUT:
		int_input = input_mgr->getIntInput(event.args);
		break;
	case CRIT_ERR:
		renderer->errPrint("placeholder");
		break;
	default:
		renderer->errPrint("[DEBUG] GameEngine has recieved an undefined event\n");
		break;
	}
}
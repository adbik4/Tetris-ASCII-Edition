#include "GameEngine.h"

using namespace std;

void GameEngine::startGame() {
	running_ = true;

	renderer->showMenu();
	int option = input_mgr->getIntInput(1, 3);
	switch (option) {
	case 1:
		renderer->print("GAMEPLAY\n");
		time_mgr->startClock();
		break;
	case 2:
		renderer->print("SETTINGS\n");
		break;
	case 3:
		stop_flag = true;
		break;
	}
}

void GameEngine::update() {
	renderer->print("gameplay\n");
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
	if (event.id == CLK) {
		update();
	}
}
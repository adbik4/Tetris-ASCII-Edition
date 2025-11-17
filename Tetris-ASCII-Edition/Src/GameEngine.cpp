#include "GameEngine.h"

void GameEngine::startGame() {
	running_ = true;

	renderer->showMenu();
	int option = input_mgr->getIntInput(1, 3);
	switch (option) {
	case 1:
		cout << "GAMEPLAY\n";
		time_mgr->startClock();
		break;
	case 2:
		cout << "SETTINGS\n";
		break;
	case 3:
		stop_flag = true;
		break;
	}
}

void GameEngine::update() {
	cout << "gameplay\n";
	stop_flag = true;
}

void GameEngine::stopGame() {
	if (!running_) return;
	running_ = false;
	time_mgr->stopClock();
	renderer->showEndScreen();

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
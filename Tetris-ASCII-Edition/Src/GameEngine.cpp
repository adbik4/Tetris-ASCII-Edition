#include "GameEngine.h"

using namespace std;

void GameEngine::startGame() {
	state.running = true;

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
		state.stop_flag = true;
		break;
	}
}

void GameEngine::update() {
	// INPUT
	// char k_input = input_mgr->getKeyboardInput();

	// GAME LOGIC
	renderer->print("tetris");

	// RENDER OUTPUT
	//renderer->updateScreen();
}

void GameEngine::stopGame() {
	if (!state.running) return;
	state.running = false;
	renderer->showEndScreen();
	time_mgr->stopClock();
}

void GameEngine::nextLevel() {
	state.level++;
}

// This function conducts the main mediation logic
void GameEngine::notify (const Event& event) {
	if (event.id == CLK) {
		update();
		state.frame++;
	}
}
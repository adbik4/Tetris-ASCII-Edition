#include "GameEngine.h"

#include "TimeManager.h"

void GameEngine::startGame() {
	clock->startClock();
}

void GameEngine::stopGame() {
	clock->stopClock();
}

// This function conducts the main mediation logic
void GameEngine::notify (const Event& event) {
	if (event.id == CLK) {
		cout << "tick\n";
	}
}
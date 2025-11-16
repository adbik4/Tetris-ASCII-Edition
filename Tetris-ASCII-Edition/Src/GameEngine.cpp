#include "GameEngine.h"
#include "TimeManager.h"

void GameEngine::startGame() {
	tm->startClock();
}

void GameEngine::stopGame() {
	tm->stopClock();
}

// This function conducts the main mediation logic
void GameEngine::notify (const Event& event) {
	if (event.id == CLK) {
		cout << "tick\n";
	}
}
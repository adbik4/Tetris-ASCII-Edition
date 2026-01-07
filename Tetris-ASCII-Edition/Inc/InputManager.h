#pragma once
#include <GameEngine.h>
#include "WindowManager.h"

class InputManager {
private:
	weak_ptr<GameEngine> eng;
	weak_ptr<WindowManager> wm;

public:
	InputManager(const shared_ptr<GameEngine>& engine_ptr, const shared_ptr<WindowManager>& win_ptr) : eng(engine_ptr), wm(win_ptr) {}

	int getKeyboardInput(const uint8_t win_id);
	int getIntInput(const tuple<int, int>& bounds);
	int waitForAnyKey();
};
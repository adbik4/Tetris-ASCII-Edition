#pragma once
#include <GameEngine.h>
#include "WindowManager.h"

class InputManager {
private:
	shared_ptr<GameEngine> engine;
	shared_ptr<WindowManager> win_mgr;

public:
	InputManager(const shared_ptr<GameEngine>& engine_ptr, const shared_ptr<WindowManager>& win_ptr) : engine(engine_ptr), win_mgr(win_ptr) {}

	int getKeyboardInput(const uint8_t win_id);
	int getIntInput(const tuple<int, int>& bounds);
	int waitForAnyKey();
};
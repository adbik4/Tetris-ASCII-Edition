#pragma once
#include <memory>
#include <tuple>
#include "GameEngine.h"

class GameEngine;
class WindowManager;

// This object is responsible for obtaining user input from the keyboard
class InputManager {
private:
	weak_ptr<GameEngine> eng;
	weak_ptr<WindowManager> wm;

public:
	InputManager(const shared_ptr<GameEngine>& engine_ptr, const shared_ptr<WindowManager>& win_ptr) : eng(engine_ptr), wm(win_ptr) {};

	int getKeyboardInput();
	int waitForAnyKey();
};
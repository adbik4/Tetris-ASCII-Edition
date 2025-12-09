#pragma once
#include <string>
#include <curses.h>
#include "GameEngine.h"
#include "WindowManager.h"

using namespace std;
class GameEngine;

class GameRenderer {
private:
	shared_ptr<GameEngine> engine;
	shared_ptr<WindowManager> win_mgr;

public:
	GameRenderer(const shared_ptr<GameEngine>& engine_ptr, const shared_ptr< WindowManager>& win_ptr) : engine(engine_ptr), win_mgr(win_ptr) {}
	~GameRenderer() {}

	void windowPrint(const int& win_id, const string& str);
	void showMenu();
	void showEndScreen();
	void clearWindow(const int& win_id);
};
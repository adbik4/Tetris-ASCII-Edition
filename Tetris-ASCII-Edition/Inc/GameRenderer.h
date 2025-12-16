#pragma once
#include <string>
#include <curses.h>
#include "GameEngine.h"
#include "WindowManager.h"
#include <array>

using namespace std;
class GameEngine;

class GameRenderer {
private:
	shared_ptr<GameEngine> engine;
	shared_ptr<WindowManager> win_mgr;

	WINDOW* game_win = win_mgr->getWindow(GAME_WIN);		// a reference to the game window
	std::array<char, 3> block_buff;							// a buffer for a single ascii tile


	void render_tile(const char tile);
	void render_piece(const uint8_t x, const uint8_t y);

public:
	GameRenderer(const shared_ptr<GameEngine>& engine_ptr, const shared_ptr< WindowManager>& win_ptr) :
		engine(engine_ptr),
		win_mgr(win_ptr),
		block_buff({ '.', '.', '\0' })
	{}

	~GameRenderer() {}

	// graphics
	void renderFrame();
	void blink();

	// utility
	void windowPrint(const int& win_id, const string& str);
	void errPrint(const string& str);
	void showMenu();
	void initGameUI();
	void initSettingsUI();
	void showEndScreen();
};
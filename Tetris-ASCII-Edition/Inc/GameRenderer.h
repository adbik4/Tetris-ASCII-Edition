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
	weak_ptr<GameEngine> eng;
	weak_ptr<WindowManager> wm;

	WINDOW* game_win;		// a reference to the game window


	void render_tile(const char tile);
	void render_piece(const Tetromino& piece, const uint8_t x, const uint8_t y, const int& win_id=GAME_WIN);

public:
	GameRenderer(const shared_ptr<GameEngine>& engine_ptr, const shared_ptr<WindowManager>& win_ptr) :
		eng(engine_ptr),
		wm(win_ptr)
	{
		auto win_mgr = wm.lock();
		game_win = win_mgr->getWindow(GAME_WIN);
	}

	~GameRenderer() {}

	// graphics
	void renderFrame();
	void refreshGameUI();
	void refreshMenuUI();
	void refreshSettingsUI();

	void flashEffect();
	void lineClearEffect(vector<uint8_t> lines, uint16_t score);

	// utility
	void windowPrint(const int& win_id, const string& str);
	void windowPrintAtPos(const int& win_id, const int& x, const int& y, const string& str);
	void windowReset(const int& win_id);
	void errPrint(const string& str);
	void initGameUI();
	void initMenuUI();
	void initSettingsUI();
	void showTitleScreen();
	void showEndScreen(const GameState& state);
};

inline constexpr array<char, 1507> title_art {
"oooooooooooooooooo ooooooooooooooooo oooooooooooooooooo ooooooooooooooo     oooooooo    oooooooooooooo\n"
"oooooooooooooooooo ooooooooooooooooo oooooooooooooooooo ooooooooooooooooo   oooooooo  ooooooooooooooo\n"
"oooooooooooooooooo ooooooooooooooooo oooooooooooooooooo oooooooooooooooooo  oooooooo ooooooooooooooo\n"
"oooooooooooooooooo ooooooooooooooooo oooooooooooooooooo oooooooooooooooooo  ooooooo ooooooooooooooo\n"
"     oooooooo      ooooooo                oooooooo      ooooooo  ooooooooo  oooooooo ooooooooo\n"
"     oooooooo      ooooooooooooooo        oooooooo      ooooooooooooooooo   oooooooo oooooooooo\n"
"     oooooooo      ooooooooooooooo        oooooooo      oooooooooooooooo    oooooooo   ooooooooo\n"
"     oooooooo      oooooooooooooo         oooooooo      oooooooooooooooo    oooooooo    oooooooooo\n"
"     oooooooo      ooooooooooooo          oooooooo      ooooooo ooooooooo   oooooooo     oooooooooo\n"
"     oooooooo      oooooooo               oooooooo      ooooooo  ooooooooo  oooooooo       ooooooooo\n"
"     oooooooo      oooooooo               oooooooo      oooooooo   oooooooo oooooooo        oooooooooo\n"
"     oooooooo      oooooooooooooooooo     oooooooo      oooooooo    ooooooo oooooooo ooooooooooooooooo\n"
"     oooooooo      oooooooooooooooooooo   oooooooo      oooooooo     oooooo oooooooo ooooooooooooooooo\n"
"     oooooooo      ooooooooooooooooooooo  oooooooo      oooooooo      ooooo oooooooo ooooooooooooooooo\n"
"     oooooooo        oooooooooooooooooooo oooooooo      oooooooo       oooo oooooooo ooooooooooooooo" 
};
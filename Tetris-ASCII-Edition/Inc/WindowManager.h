#pragma once

#include <curses.h>

using namespace std;

#define MAIN_MENU 0

class WindowManager {
private:
	WINDOW* menu_win;
	WINDOW* settings_win;
	WINDOW* game_win;

	void initTerm();
	void deinitTerm();
	WINDOW* makeMenuWindow();

public:
	WindowManager() {
		initTerm();
		menu_win = makeMenuWindow();
	}
	~WindowManager() { deinitTerm(); }

	WINDOW* getWindow(const int& win_id);
};
#pragma once

#include <curses.h>

using namespace std;

#define MAIN_MENU	0
#define INPUT_WIN	2
#define ERR_WIN		1

class WindowManager {
private:
	WINDOW* menu_win;
	WINDOW* input_win;
	WINDOW* err_win;
	WINDOW* game_win;

	void initTerm();
	void deinitTerm();
	WINDOW* createNewWindow(const int& height, const int& width, const int& starty, const int& startx);
	WINDOW* makeMenuWindow();
	WINDOW* makeInputWindow();
	WINDOW* makeErrorWindow();

public:
	WindowManager() {
		initTerm();
		menu_win = makeMenuWindow();
		input_win = makeInputWindow();
		err_win = makeErrorWindow();
	}
	~WindowManager() { deinitTerm(); }

	void showBorder(const int& win_id);
	void clearBorder(const int& win_id);
	void clearContents(const int& win_id);
	void clearWindow(const int& win_id);
	WINDOW* getWindow(const int& win_id);
};